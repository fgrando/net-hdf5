#include <stdio.h>
#include <stdlib.h> // malloc

#include "net.h"
#include "sink.h"
#include "print.h"
#include "handler.h"
#include "persistance.h"


#include "net.h"
#include "sink.h"
#include "print.h"
#include "handler.h"

void idle_function(net_server_config_t *cfg, net_client_handler_args_t *args)
{
    static int countdown = 0;
    PRINT_DBG("Idle... %d", countdown);
    if (countdown > 15)
        *(cfg->run_server_ptr) = 0;

    PRINT_DBG("keep running? %d", *(cfg->run_server_ptr));
    countdown++;
}

#define demo 0
int main(int argc, char* argv[])
{

    int alive = 1;
    persistance_cfg_t file;
#if demo
    net_client_handler_args_t handler_args = {0};
    net_client_handler_t handler = &net_handle_echo_client;
#else
    handler_args_t handler_args;
    handler_set_file_sink(&handler_args);

    net_client_handler_t handler = (net_client_handler_t*)&handler_client;
    net_client_handler_t idle = &idle_function;
#endif

    // parse configuration
    if (parse_config((argc > 1 ? argv[1] : "nethdf5.conf"), &file))
    {
        PRINT_ERR("failed to parse configs");
        return 1;
    }
    net_client_sock_t *client_sockets =
        (net_client_sock_t*) malloc(sizeof(net_client_sock_t) * file.max_clients); // max clients

    // configs for this server
    net_server_config_t server_configs;
    server_configs.run_server_ptr = &alive;
    server_configs.sock_clients = client_sockets;
    server_configs.sock_clients_max = file.max_clients;
    server_configs.timeout_us = (1000000)*3; // seconds

    // spin server and handle clients
    server_configs.sock_server = net_open(file.port);

#if demo
    int ret = net_server(&server_configs,
                         &handler,
                         (net_client_handler_args_t*)&handler_args,
                         &idle);
#else
    int ret = net_server(&server_configs,
                         &handler,
                        &handler_args,
                        0);
#endif
    net_close(server_configs.sock_server);
    net_finish();

    free(client_sockets);
    client_sockets = 0;

    return 0;
}