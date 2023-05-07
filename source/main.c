#include <stdio.h>

extern int parse_config(char *filepath, int* port);

#include "net.h"
#include "sink.h"
#include "print.h"
#include "handler.h"

extern void handler_client2(net_client_handler_args_t *args);

int main(int argc, char* argv[])
{
    int server_port = 0;
#if 0
    net_client_handler_args_t handler_args = {0};
    net_client_handler_t handler = &net_handle_echo_client;
#else
    handler_args_t handler_args = handler_get_file_sink();
    net_client_handler_t handler = &handler_client;
#endif

    // parse configuration
    if (parse_config((argc > 1 ? argv[1] : "nethdf5.conf"), &server_port))
    {
        PRINT_ERR("failed to parse configs");
        return 1;
    }

    // spin server and handle clients
    int server_socket = net_open(server_port);
    net_listen(server_socket, 1, &handler, &handler_args);
    net_close(server_socket);
    net_finish();

    return 0;
}