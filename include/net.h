#ifndef NETWORK_HEADER_
#define NETWORK_HEADER_

#include "print.h"

typedef  void* NetLibSessionPtr; /* obscure pointer */

#define NET_BADSOCK -1

typedef struct {
   int sock;
   char *end; // end of this struct
} net_client_handler_args_t;

typedef void (*net_client_handler_t)(net_client_handler_args_t *args);

int net_open(int port);
int net_close(int sock);
int net_finish();
int net_listen(int sock, int max, net_client_handler_t *fptr, net_client_handler_args_t *args);
int net_recv(int sock, char* recvbuf, int recvbuflen);
int net_send(int sock, char* buff, int len);

void net_handle_echo_client(net_client_handler_args_t *args);

#endif