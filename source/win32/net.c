#include "net.h"
#include "print.h"

#undef UNICODE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
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

    if (ret != 0) {
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
            return NET_BADSOCK;
        }
    }
    PRINT_INF("opening tcp socket to port %d", port);

    char *peername = NULL;
    int ret = 0;
    struct addrinfo* result = NULL;
    struct addrinfo* ptr = NULL;
    struct addrinfo hints;
    char portStr[7] = { 0 };/* max port is 654321\0 = 7 bytes */

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
    if (ret != 0) {
        PRINT_ERR("getaddrinfo error %d", ret);
        net_print_WsaError();
        WSACleanup();
        return NET_BADSOCK;
    }

    // Create a SOCKET for the server to listen for client connections.
    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock == INVALID_SOCKET) {
        PRINT_ERR("socket failed with error: %d",ret);
        net_print_WsaError();
        freeaddrinfo(result);
        WSACleanup();
        return NET_BADSOCK;
    }
    PRINT_DBG("socket open %d", sock);

     // Setup the TCP listening socket
     ret = bind(sock, result->ai_addr, (int)result->ai_addrlen);
     if (ret == SOCKET_ERROR) {
        PRINT_ERR("bind failed with error: %d", ret);
        net_print_WsaError();
        freeaddrinfo(result);
        closesocket(sock);
        WSACleanup();
        return NET_BADSOCK;
    }
    PRINT_DBG("bind complete");

    freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        PRINT_ERR("Unable to connect to server!");
        WSACleanup();
        return NET_BADSOCK;
    }

    return sock;
}

int net_listen(int sock, int max, net_client_handler_t *fptr, net_client_handler_args_t *args)
{
    PRINT_DBG("listen to clients...");

    int ret = listen(sock, SOMAXCONN);
    if (ret == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    PRINT_DBG("wait connections...");
    int client_sock = accept(sock, NULL, NULL);

    args->sock = client_sock;

    PRINT_DBG("handle client %d", client_sock);
    (*fptr)(args);
}

int net_recv(int sock, char* recvbuf, int recvbuflen)
{
    PRINT_DBG("recv sock %d", sock);
    int bytes = recv(sock, recvbuf, recvbuflen, 0);
    if (bytes < 0)
    {
        PRINT_ERR("recv failed with error: %d", bytes);
        net_print_WsaError();
        closesocket(sock);
    }
    return bytes;
}

int net_send(int sock, char* buff, int len)
{
    PRINT_DBG("send sock %d", sock);
    int bytes = send(sock, buff, len, 0);
    if (bytes < 0)
    {
        PRINT_ERR("send failed with error: %d", bytes);
        net_print_WsaError();
        closesocket(sock);
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
    do {
        ret = recv(args->sock, recvbuf, recvbuflen, 0);
        if (ret > 0) {
            printf("Bytes received: %d\n", ret);

        // Echo the buffer back to the sender
            int iSendResult = send(args->sock, recvbuf, ret, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(args->sock);
                WSACleanup();
                break;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (ret == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(args->sock);
            WSACleanup();
            break;
        }
     } while (ret > 0);
}