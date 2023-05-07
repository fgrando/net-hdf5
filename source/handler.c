#include <string.h>

#include "handler.h"
#include "sink.h"
#include "net.h"

handler_args_t handler_get_file_sink()
{
    handler_args_t args;
    args.sink.open = file_open;
    args.sink.close = file_close;
    args.sink.append = file_append;
    strncat(args.name, "to-file", sizeof(args.name)-1);
    return args;
}

handler_args_t handler_get_hdf5_sink()
{
    handler_args_t args;
    args.sink.open = hdf5_open;
    args.sink.close = hdf5_close;
    args.sink.append = hdf5_append;
    strncat(args.name, "to-hdf5", sizeof(args.name)-1);
    return args;
}

int parse_recv(char* buffer, int len);

void handler_client(handler_args_t *args)
{
    PRINT_DBG("handler %s sock %d", args->name, args->sock);
    char recvbuf[1000];
    int recvbuflen = 1000;

    int ret = 0;
    do {
        ret = net_recv(args->sock, recvbuf, recvbuflen);
        if (parse_recv(recvbuf, ret))
        {
            break;
        }

        int bytes = net_send(args->sock, recvbuf, ret);
        printf("Bytes sent: %d\n", bytes);
    } while (ret > 0);
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
