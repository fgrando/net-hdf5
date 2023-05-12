#include <string.h>

#include "handler.h"
#include "sink.h"
#include "net.h"
#include "timestamp.h"
#include "netcmd.h"

#define SEPARATOR " "

void handler_set_file_sink(handler_args_t *args)
{
    args->sink.open = file_open;
    args->sink.close = file_close;
    args->sink.append = file_append;
    memset(args->name, 0, sizeof(args->name));
    strncat(args->name, "to-file", sizeof(args->name)-1);
}


void handler_set_hdf5_sink(handler_args_t *args)
{
    args->sink.open = hdf5_open;
    args->sink.close = hdf5_close;
    args->sink.append = hdf5_append;
    memset(args->name, 0, sizeof(args->name));
    strncat(args->name, "to-hdf5", sizeof(args->name)-1);
}

int parse_recv(char* buffer, int len, handler_args_t* data);

void handler_client(net_client_handler_args_t *args)
{
    handler_args_t* private = (handler_args_t*)args->private_ptr;

    PRINT_DBG("handler %s sock %d", private->name, args->sock);
    char recvbuf[1000] = {0};
    int recvbuflen = 1000;

    args->status = NET_SOCK_OK;
    int ret = 0;
    ret = net_recv(args->sock, recvbuf, recvbuflen);
    if (ret < 1)
    {
        args->status = NET_SOCK_FAIL;
        PRINT_INF("%s","socket fail");
        return;
    }
    PRINT_DBG("%d >>> %s", args->sock, recvbuf);
    if (parse_recv(recvbuf, ret, private))
    {
        // command is bad. reply nack
        memset(recvbuf, 0, sizeof(recvbuf));    
        ret = netcmd_to_str(netcmd_nack, recvbuf, sizeof(recvbuf));
    }
    else
    {
        // command is good. reply ack
        memset(recvbuf, 0, sizeof(recvbuf));    
        ret = netcmd_to_str(netcmd_ack, recvbuf, sizeof(recvbuf));
    }

    PRINT_DBG("%d <<< %s", args->sock, recvbuf);
    ret = net_send(args->sock, recvbuf, ret);    
    if (ret < 1)
    {
        args->status = NET_SOCK_FAIL;
        PRINT_ERR("%s","socket fail");
        return;
    }
}

int parse_recv(char* buffer, int len, handler_args_t* data)
{
    char *cmd = strtok(buffer, SEPARATOR);

    if(netcmd_str_equal(netcmd_abort, cmd, strlen(cmd)))
    {
        PRINT_INF("Remote shutdown activated");
        *data->keep_running = 0;
        return 0;
    }

    return 1;
}
