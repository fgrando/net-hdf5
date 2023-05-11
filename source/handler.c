#include <string.h>

#include "handler.h"
#include "sink.h"
#include "net.h"

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

int parse_recv(char* buffer, int len);

void handler_client(handler_args_t *args)
{
    PRINT_DBG("handler %s sock %d", args->name, args->net.sock);
    char recvbuf[1000];
    int recvbuflen = 1000;

    args->net.status = NET_SOCK_OK;
    int ret = 0;
    ret = net_recv(args->net.sock, recvbuf, recvbuflen);
    if (ret < 1)
    {
        args->net.status = NET_SOCK_FAIL;
        PRINT_INF("%s","socket fail");
        return;
    }
    if (parse_recv(recvbuf, ret))
    {
        PRINT_INF("%s","socket close");
        //args->net.status = NET_SOCK_CLOSED;
        return;
    }

    int bytes = net_send(args->net.sock, recvbuf, ret);
    printf("Bytes sent: %d\n", bytes);

    if (bytes < 1)
    {
        //args->net.status = NET_SOCK_FAIL;
        PRINT_INF("%s","socket fail");
        return;
    }
}

int parse_recv(char* buffer, int len)
{
    if (len < 1)
    {
        printf("Connection closing...\n");
        return 1;
    }
    printf("Bytes received: %d %s\n", len, buffer);
    return 0;
}
