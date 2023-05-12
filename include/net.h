#ifndef NETWORK_HEADER_
#define NETWORK_HEADER_

#include "print.h"

typedef int net_client_sock_t;
typedef struct
{
   net_client_sock_t sock;
   char ip[15];
}net_client_t;


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
   int *serve;                  // flag to keep the server running
   int sock_server;                 // server socket (listen to connections)
   net_client_t *clients; // client sockets
   int clients_max;            // total number of available client sockets
   long timeout_us;                 // timeout in microseconds (1s = 1000000)
} net_server_config_t;

/**
 * @brief Parameters forwarded to network handler (typically the client).
 *
 * Detailed explanation.
 */
typedef struct
{
   int sock; /**< Client socket to be handled. */
   int status; /**< Return status. If not OK results in the socket closure and client disconnection */
   void *private_ptr; /**< Obscure pointer to private handler data. */
} net_client_handler_args_t;

// this function is called every time a client socket has changes
typedef void (*net_client_handler_t)(net_client_handler_args_t *args);

// this function is called every time the timer expires and no socket was triggered
typedef void (*net_idle_hander_t)(net_server_config_t *cfg, net_client_handler_args_t *args);

int net_open(int port);
int net_close(int sock);
int net_finish();

/**
 * @brief Network server
 *
 * Handle client connections using the handler function @p fptr_handler.
 * The handler parameters @p args are forwarded to the client handler.
 * Idle function has access to client @p args and server parameters @p cfg.
 * Idle function only runs once the timeout expires and we do not have chat with clients.
 * This function loops until flag is disable.
 * @param cfg Serve parameters.
 * @param fptr_handler Client connection handler.
 * @param args Client connection handler argument.
 * @param fptr_idle Optional. Useful for maintenance functions.
 * @return 0 if successful.
 */
int net_server(net_server_config_t *cfg,
               net_client_handler_t *fptr_handler,
               net_client_handler_args_t *args,
               net_idle_hander_t *fptr_idle);

int net_recv(int sock, char *recvbuf, int recvbuflen);
int net_send(int sock, char *buff, int len);

void net_handle_echo_demo(net_client_handler_args_t *args);

#endif