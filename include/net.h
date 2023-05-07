#ifndef NETWORK_HEADER_

#include "print.h"

typedef  void* NetLibSessionPtr; /* obscure pointer */

#define NET_BADSOCK -1

typedef struct {
   int sock;
   char *end;
} net_client_handler_args_t;

typedef void (*net_client_handler_t)(net_client_handler_args_t *args);

int net_open(int port);
int net_close(int sock);
int net_finish();
int net_listen(int sock, int max, net_client_handler_t *fptr, net_client_handler_args_t *args);
void net_handle_client(net_client_handler_args_t *args);

#endif