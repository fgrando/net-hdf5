#include <string.h>

#include "handler.h"
#include "sink.h"
#include "net.h"
#include "timestamp.h"

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
    char recvbuf[1000] = {0};
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
    PRINT_DBG("%d >>> %s", args->net.sock, recvbuf);
    if (parse_recv(recvbuf, ret))
    {
        PRINT_INF("%s","socket close");
        args->net.status = NET_SOCK_CLOSED;
        return;
    }

    memset(recvbuf, 0, sizeof(recvbuf));
    timestamp(recvbuf, sizeof(recvbuf), 0);
    PRINT_DBG("%d <<< %s", args->net.sock, recvbuf);
    ret = net_send(args->net.sock, recvbuf, ret);

    if (ret < 1)
    {
        args->net.status = NET_SOCK_FAIL;
        PRINT_ERR("%s","socket fail");
        return;
    }
}

int parse_recv(char* buffer, int len)
{
    if (len < 1)
    {
        return 1;
    }
    return 0;
}
