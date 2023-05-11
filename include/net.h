#ifndef NETWORK_HEADER_
#define NETWORK_HEADER_

#include "print.h"

typedef void *NetLibSessionPtr; /* obscure pointer */
typedef int net_client_sock_t;

#define NET_INVALID_SOCK -1

typedef enum
{
   NET_SOCK_INVALID,
   NET_SOCK_OK,
   NET_SOCK_FAIL,
   NET_SOCK_CLOSED,
} net_sock_status_t;

typedef struct
{
   int *run_server_ptr;             // variable that keeps sever running
   int sock_server;                 // server socket (listen to connections)
   net_client_sock_t *sock_clients; // client sockets
   int sock_clients_max;            // total number of available client sockets
   long timeout_us;                 // timeout in microseconds (1s = 1000000)
} net_server_config_t;

typedef struct
{
   int sock;
   int status;
   void *private_ptr;
} net_client_handler_args_t;

// this function is called every time a client socket has changes
typedef void (*net_client_handler_t)(net_client_handler_args_t *args);

// this function is called every time the timer expires and no socket was triggered
typedef void (*net_idle_hander_t)(net_server_config_t *cfg, net_client_handler_args_t *args);

int net_open(int port);
int net_close(int sock);
int net_finish();

int net_server(net_server_config_t *cfg,
               net_client_handler_t *fptr_handler,
               net_client_handler_args_t *args,
               net_idle_hander_t *fptr_idle);

int net_recv(int sock, char *recvbuf, int recvbuflen);
int net_send(int sock, char *buff, int len);

void net_handle_echo_client(net_client_handler_args_t *args);

#endif