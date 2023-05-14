#include <string.h>
#include <stdlib.h>

#include "handler.h"
#include "sink.h"
#include "net.h"
#include "timestamp.h"
#include "netcmd.h"

#define SEPARATOR ' '
#define HANDLER_MAX_MESSAGE_LEN_BYTES 3000

void handler_set_file_sink(handler_args_t *args)
{
    memset(&args->sink, 0, sizeof(args->sink));
    args->sink.open = file_open;
    args->sink.close = file_close;
    args->sink.append = file_append;
    memset(args->name, 0, sizeof(args->name));
    strncat(args->name, "to-file", sizeof(args->name)-1);
}

void handler_set_hdf5_sink(handler_args_t *args)
{
    memset(&args->sink, 0, sizeof(args->sink));
    args->sink.open = hdf5_open;
    args->sink.close = hdf5_close;
    args->sink.append = hdf5_append;
    memset(args->name, 0, sizeof(args->name));
    strncat(args->name, "to-hdf5", sizeof(args->name)-1);
}

int get_argument(char* src, int begin, char*dst, int max)
{
    memset(dst, 0, max);
    int quotes = 0; // spaces into a quoted string are not separators

    int src_len = strlen(src);
    if (begin >= src_len)
    {
        return -1;
    }
    
    // ignore trailling spaces
    while(src[begin] == SEPARATOR)
    {
        begin++;
    }

    // if it starts with quotes, move to next char but active quote marks
    if (src[begin] == '\"')
    {
        begin++;
        quotes = 1;
    }

    int sz = 0;
    int i = begin;
    for(;i < src_len; i++)
    {
        if (!quotes && src[i] == SEPARATOR)
        {
            sz = i - begin;
            i++; // skip blank char next time
            break;
        }

        if (src[i] == '\"')
        {
            quotes = 0;
            sz = i - begin;
            i++; // skip quote char next time
            break;
        }

        if (i+1 == src_len)
        {
            i++; // skip quote char next time
            sz = i - begin;
            break;
        }
    }

    if (sz > max)
    {
        PRINT_ERR("Not enough space to save substring (req %d, max %d)", sz, max);
        return -1;
    }
    strncat(dst, &src[begin], sz);

    return i;
}

int handle_create(char* buffer, int len, net_client_handler_args_t *conn, handler_args_t* data)
{
    char filepath[HANDLER_MAX_PATH_NAME] = {0};
    char metadata[HANDLER_MAX_PATH_NAME] = {0};
    char arg[HANDLER_MAX_PATH_NAME] = {0};
    int idx = 0;


    // create
    idx = get_argument(buffer, idx, arg, sizeof(arg));
    if(idx < 0 || netcmd_str_equal(netcmd_create, arg) == 0)
    {
        return 1;
    }
    PRINT_DBG("cmd: %s", arg);

    // filepath
    idx = get_argument(buffer, idx, arg, sizeof(arg));
    if(idx < 0)
    {
        return 1;
    }
    strncat(filepath, arg, sizeof(filepath)-1);
    PRINT_DBG("filepath: %s", arg);

    // metadata
    idx = get_argument(buffer, idx, arg, sizeof(arg));
    if(idx < 0)
    {
        return 1;
    }
    strncat(metadata, arg, sizeof(metadata)-1);
    PRINT_DBG("meta: %s", arg);

    return data->sink.open(&data->sink.session, filepath);
}

int handle_append(char* buffer, int len, net_client_handler_args_t *conn, handler_args_t* data)
{
    char label[HANDLER_MAX_PATH_NAME] = {0};
    float time;
    char arg[HANDLER_MAX_PATH_NAME] = {0};
    int idx = 0;

    // append
    idx = get_argument(buffer, idx, arg, sizeof(arg));
    if(idx < 0 || netcmd_str_equal(netcmd_append, arg) == 0)
    {
        return 1;
    }
    PRINT_DBG("cmd: %s", arg);

    // label
    idx = get_argument(buffer, idx, arg, sizeof(arg));
    if(idx < 0)
    {
        return 1;
    }
    strncat(label, arg, sizeof(label)-1);
    PRINT_DBG("label: %s", arg);

    // time
    idx = get_argument(buffer, idx, arg, sizeof(arg));
    if(idx < 0)
    {
        return 1;
    }
    time = atof(arg);
    PRINT_DBG("time: %f %s", time, arg);

    return data->sink.append(&data->sink.session, label, time, &buffer[idx], len-idx);
}

int parse_recv(char* buffer, int len, net_client_handler_args_t *conn, handler_args_t* data)
{
    if(netcmd_str_equal(netcmd_abort, buffer))
    {
        PRINT_INF("Remote shutdown activated");
        *data->keep_running = 0;
        conn->status = NET_SOCK_CLOSED;
        return 0;
    }

    if(netcmd_str_equal(netcmd_create, buffer))
    {
        return handle_create(buffer, len, conn, data);
    }

    if(netcmd_str_equal(netcmd_append, buffer))
    {
        return handle_append(buffer, len, conn, data);
    }

    if(netcmd_str_equal(netcmd_close, buffer))
    {
        return data->sink.close(&data->sink.session);
    }

    return 1;
}

void shutdown_and_fail(net_client_handler_args_t *conn, handler_args_t* data)
{
    conn->status = NET_SOCK_FAIL;
    data->sink.close(&data->sink.session);
}

void handler_client(net_client_handler_args_t *args)
{
    handler_args_t* private = (handler_args_t*)args->private_ptr;

    PRINT_DBG("handler %s sock %d", private->name, args->sock);
    char buffer[HANDLER_MAX_MESSAGE_LEN_BYTES] = {0};
    int ret = 0;

    args->status = NET_SOCK_OK; // default is success

    ret = net_recv(args->sock, buffer, sizeof(buffer));
    if (ret < 1)
    {
        PRINT_INF("%s","socket fail");
        shutdown_and_fail(args, private);
        return;
    }
    PRINT_DBG("%d >>> %s", args->sock, buffer);
    
    if (parse_recv(buffer, ret, args, private))
    {
        // command is bad. reply nack
        memset(buffer, 0, sizeof(buffer));    
        ret = netcmd_to_str(netcmd_nack, buffer, sizeof(buffer));
    }
    else
    {
        // command is good. reply ack
        memset(buffer, 0, sizeof(buffer));    
        ret = netcmd_to_str(netcmd_ack, buffer, sizeof(buffer));
    }

    ret = net_send(args->sock, buffer, ret);    
    if (ret < 1)
    {
        PRINT_INF("%s","socket fail");
        shutdown_and_fail(args, private);
        return;
    }
    PRINT_DBG("%d <<< %s", args->sock, buffer);
}
