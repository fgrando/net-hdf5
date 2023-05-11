#include "net.h"
#include "print.h"

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")


void net_print_WsaError()
{
    LPSTR errStr = NULL;
    int ret = 0;
    int len = 0;

    ret = WSAGetLastError();
    len = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, ret, 0, (LPSTR)&errStr, 0, 0);
    PRINT_ERR("WSAErr %d: %s", ret, errStr);
    LocalFree(errStr);
}

int net_init()
{
    WSADATA wsaData;
    int ret;
    ret = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (ret != 0)
    {
        net_print_WsaError();
    }
    return ret;
}

int net_open(int port)
{
    int sock = 0;
    static int initialized = 0;

    if (!initialized)
    {
        if (net_init() != 0)
        {
            return NET_INVALID_SOCK;
        }
    }
    PRINT_INF("opening tcp socket to port %d", port);

    char *peername = NULL;
    int ret = 0;
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
    char portStr[7] = {0}; /* max port is 654321\0 = 7 bytes */

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    ret = _snprintf_s(portStr, sizeof(portStr), sizeof(portStr), "%d", port);
    if (ret >= sizeof(portStr))
    {
        PRINT_ERR("invalid port %d", port);
        return ret;
    }

    ret = getaddrinfo(peername, portStr, &hints, &result);
    if (ret != 0)
    {
        PRINT_ERR("getaddrinfo error %d", ret);
        net_print_WsaError();
        WSACleanup();
        return NET_INVALID_SOCK;
    }

    // Create a SOCKET for the server to listen for client connections.
    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET)
    {
        PRINT_ERR("socket failed with error: %d", ret);
        net_print_WsaError();
        freeaddrinfo(result);
        WSACleanup();
        return NET_INVALID_SOCK;
    }
    PRINT_DBG("socket open %d", sock);

    // Setup the TCP listening socket
    ret = bind(sock, result->ai_addr, (int)result->ai_addrlen);
    if (ret == SOCKET_ERROR)
    {
        PRINT_ERR("bind failed with error: %d", ret);
        net_print_WsaError();
        freeaddrinfo(result);
        closesocket(sock);
        WSACleanup();
        return NET_INVALID_SOCK;
    }
    PRINT_DBG("bind complete");

    freeaddrinfo(result);

    if (sock == INVALID_SOCKET)
    {
        PRINT_ERR("Unable to connect to server!");
        WSACleanup();
        return NET_INVALID_SOCK;
    }

    return sock;
}

// local function
// close the socket (if open) and free its position
int net_close_client(net_client_sock_t *clients, int pos)
{
    net_client_sock_t sock = clients[pos];
    if (sock == NET_INVALID_SOCK)
    {
        // already closed
        return 0;
    }

    PRINT_DBG("close client %d sock %d", pos, sock);
    int ret = net_close(sock);
    // free slot
    clients[pos] = NET_INVALID_SOCK;
    return ret;
}

int net_server(net_server_config_t          *cfg,
               net_client_handler_t         *fptr_handler,
               net_client_handler_args_t    *args,
               net_idle_hander_t            *fptr_idle)
{
    int ret = 0; // return code
    SOCKET accept_sock = cfg->sock_server;
    FD_SET fdSet; // set of the sockets to monitore

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = cfg->timeout_us;

    // display cfgs
    PRINT_DBG("server socket is %d", accept_sock);
    PRINT_DBG("server keep running at %p", cfg->run_server_ptr);
    PRINT_DBG("max clients %d", cfg->sock_clients_max);
    PRINT_DBG("idle function runs every %dus", timeout.tv_usec);

    ret = listen(accept_sock, cfg->sock_clients_max);
    if (ret == SOCKET_ERROR)
    {
        PRINT_ERR("listen failed with error: %ld\n", ret);
        net_print_WsaError();
        closesocket(accept_sock);
        WSACleanup();
        return 1;
    }

    // clear client slots
    for (int i = 0; i < cfg->sock_clients_max; i++)
    {
        cfg->sock_clients[i] = NET_INVALID_SOCK;
    }

    PRINT_DBG("starting main loop and wait for clients");
    while (*cfg->run_server_ptr)
    {
        int run_idle = 1;
        FD_ZERO(&fdSet);
        FD_SET(accept_sock, &fdSet);

        // add all active clients back to the reading set
        for (int i = 0; i < cfg->sock_clients_max; i++)
        {
            if (cfg->sock_clients[i] != NET_INVALID_SOCK)
            {
                FD_SET(cfg->sock_clients[i], &fdSet);
            }
        }

        ret = select(0, &fdSet, NULL, NULL, &timeout);
        if (ret == SOCKET_ERROR)
        {
            PRINT_ERR("select error: %ld\n", ret);
            net_print_WsaError();
            return 1;
        }


        if (FD_ISSET(accept_sock, &fdSet))
        {
            // we have a new joiner. Find a place.
            for (int i = 0; i < cfg->sock_clients_max; i++)
            {
                if (cfg->sock_clients[i] == NET_INVALID_SOCK)
                {
                    run_idle = 0; // this cycle we were busy
                    struct sockaddr addr = {0};
                    int address_len = sizeof(addr);
                    cfg->sock_clients[i] = accept(accept_sock, &addr, &address_len);
                    PRINT_DBG("new client (%d) ip %s", i, addr.sa_data);
                    break;
                }
            }
        }

        // test every socket to see if we have something to handle
        for (int i = 0; i < cfg->sock_clients_max; i++)
        {
            // skip unused places
            if (cfg->sock_clients[i] == NET_INVALID_SOCK)
            {
                continue;
            }
            if (FD_ISSET(cfg->sock_clients[i], &fdSet))
            {
                run_idle = 0; // this cycle we were busy
                args->status = NET_SOCK_OK;
                args->sock = cfg->sock_clients[i];

                PRINT_DBG("handle client %d in socket %d", i, args->sock);
                (*fptr_handler)(args);
                PRINT_DBG("handle client %d in socket %d status %d", i, args->sock, args->status);
                if (args->status != NET_SOCK_OK)
                {
                    net_close_client(cfg->sock_clients[i], i);
                }
            }
        }

        if (run_idle && fptr_idle)
        {
            (*fptr_idle)(cfg, args);
        }
    }

    ret = 0;
    // close everyone
    for (int i = 0; i < cfg->sock_clients_max; i++)
    {
        ret +=net_close_client(cfg->sock_clients, i);
    }

    return ret;
}

int net_recv(int sock, char *recvbuf, int recvbuflen)
{
    PRINT_DBG("recv sock %d", sock);
    int bytes = recv(sock, recvbuf, recvbuflen, 0);
    if (bytes < 0)
    {
        PRINT_ERR("recv failed with error: %d", bytes);
        net_print_WsaError();
    }
    return bytes;
}

int net_send(int sock, char *buff, int len)
{
    PRINT_DBG("send sock %d", sock);
    int bytes = send(sock, buff, len, 0);
    if (bytes < 0)
    {
        PRINT_ERR("send failed with error: %d", bytes);
        net_print_WsaError();
    }
    return bytes;
}

int net_close(int sock)
{
    int ret = closesocket(sock);
    if (ret != 0)
    {
        net_print_WsaError();
        PRINT_ERR("socket close ret %d", ret);
        return 1;
    }

    return 0;
}

int net_finish()
{
    int ret = WSACleanup();
    if (ret != 0)
    {
        PRINT_ERR("Finish failed %d", ret);
        return 1;
    }

    return 0;
}

// echo server example
void net_handle_echo_client(net_client_handler_args_t *args)
{
    PRINT_DBG("sock %d", args->sock);
    char recvbuf[1000];
    int recvbuflen = 1000;

    int ret = 0;
    // Receive until the peer shuts down the connection
    do
    {
        ret = recv(args->sock, recvbuf, recvbuflen, 0);
        if (ret > 0)
        {
            printf("Bytes received: %d\n", ret);

            // Echo the buffer back to the sender
            ret = send(args->sock, recvbuf, ret, 0);
            if (ret == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", ret);
                net_print_WsaError();
                closesocket(args->sock);
                WSACleanup();
                break;
            }
            printf("Bytes sent: %d\n", ret);
        }
        else if (ret == 0)
            printf("Connection closing...\n");
        else
        {
            printf("recv failed with error: %d\n", ret);
            net_print_WsaError();
            closesocket(args->sock);
            WSACleanup();
            break;
        }
    } while (ret > 0);
}