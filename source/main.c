#include <stdio.h>
#include <stdlib.h> // malloc

#include "net.h"
#include "sink.h"
#include "print.h"
#include "handler.h"
#include "cfg_file.h"
#include "timestamp.h"

void idle_function(net_server_config_t *cfg, net_client_handler_args_t *args)
{
    static int countdown = 0;
    char now[80] = {0};
    timestamp(now, sizeof(now), 0);
    //PRINT_DBG("%s: %d (serve %d)", now, countdown, *(cfg->serve));
    //PRINT_DBG("%c", '.');
    if (countdown > 3000)
        *(cfg->serve) = 0;
    countdown++;
}

#define demo 0
int main(int argc, char* argv[])
{
    int alive = 1;

    net_client_handler_args_t handler_args = {0};
#if demo
    net_client_handler_t handler = &net_handle_echo_demo;

    // allow clients to kill the server
    handler_args.private_ptr = (void*)&alive;
#else
    handler_args_t custom_args;
    handler_set_file_sink(&custom_args);
    custom_args.keep_running = &alive; // allow clients to kill this server
    // Send our special data structure. Our handler will know how to access the data
    handler_args.private_ptr = (void*)&custom_args;

    net_client_handler_t handler = (net_client_handler_t)&handler_client;
    net_idle_hander_t    idle = &idle_function;
#endif



    // parse configuration
    cfg_file_t file;
    if (cfg_file_load((argc > 1 ? argv[1] : "nethdf5.conf"), &file))
    {
        PRINT_ERR("failed to parse configs");
        return 1;
    }
    net_client_t *client_sockets =
        (net_client_t*) malloc(sizeof(net_client_t) * file.max_clients); // max clients

    // configs for this server
    net_server_config_t server_configs;
    server_configs.serve = &alive;
    server_configs.clients = client_sockets;
    server_configs.clients_max = file.max_clients;
    server_configs.timeout_us = (1000)*500; // miliseconds

    // spin server and handle clients
    server_configs.sock_server = net_open(file.port);

#if demo
    int ret = net_server(&server_configs,
                         &handler,
                         (net_client_handler_args_t*)&handler_args,
                         0);
#else
    int ret = net_server(&server_configs,
                        &handler,
                        (net_client_handler_args_t*)&handler_args,
                        &idle);
#endif
    net_close(server_configs.sock_server);
    net_finish();

    free(client_sockets);
    client_sockets = 0;

    return 0;
}