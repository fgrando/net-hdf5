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

int parse_recv(char* buffer, int len, net_client_handler_args_t *conn, handler_args_t* data);

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
    if (parse_recv(recvbuf, ret, args, private))
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

int number_of_arguments(char* str, int len)
{
    int found = 0;
    int quotes = 0;
    for(int i=0; i < len; i++)
    {
        if (str[i] == 0)
        {
            break;
        }

        if (str[i] != '\"')
        {
            quotes = !quotes;
        }

        if (!quotes && str[i] == ' ')
        {
            found++;
        }
    }
    return found;
}

int parse_recv(char* buffer, int len, net_client_handler_args_t *conn, handler_args_t* data)
{
    char *cmd = strtok(buffer, SEPARATOR);

    if(netcmd_str_equal(netcmd_abort, cmd, strlen(cmd)))
    {
        PRINT_INF("Remote shutdown activated");
        *data->keep_running = 0;
        conn->status = NET_SOCK_CLOSED;
        return 0;
    }

    if(netcmd_str_equal(netcmd_create, cmd, strlen(cmd)))
    {
        // create path/for/file metadata1,metadata2,metadata3) */
        if (number_of_arguments(buffer, len) < 3)
        {
            PRINT_ERR("command is incomplete [%s]", buffer);
            return 1;
        }

        char* token = cmd;
        char *path = strtok(NULL, SEPARATOR);
        char *metadata = strtok(NULL, SEPARATOR);
        PRINT_DBG("[%s][%s][%s]", cmd, path, metadata);

        if(strlen(path) > sizeof(data->session[0].filepath)-1 /*EOF*/)
        {
            PRINT_ERR("path [%s] is too big", path);
            return 1;
        }

        // search to see if this session already exists
        for(int i = 0; i < data->session_max; i++)
        {
            if (strncmp(path, data->session[i].filepath, sizeof(data->session[i].filepath)) == 0)
            {
                PRINT_ERR("path [%s] is already exists", path);
                return 1;
            }
        }

        // search for a empty spot
        for(int i = 0; i < data->session_max; i++)
        {
            if (strlen(data->session[i].filepath) == 0)
            {
                strncat(data->session[i].filepath, path, sizeof(data->session[i].filepath)-1);
                // open the fd
                return 0;
            }
        }

        return 1;
    }

    if(netcmd_str_equal(netcmd_append, cmd, strlen(cmd)))
    {
        /* append path/for/file label timestamp buffer_len buffer */
        char* token = cmd;
        char *path = strtok(NULL, SEPARATOR);
        char *label = strtok(NULL, SEPARATOR);
        PRINT_DBG("[%s][%s][%s]", cmd, path, label);

        if(strlen(path) > sizeof(data->session[0].filepath)-1 /*EOF*/)
        {
            PRINT_ERR("path [%s] is too big", path);
            return 1;
        }

        // search to see if this session already exists
        int index = -1;
        for(int i = 0; i < data->session_max; i++)
        {
            if (strncmp(path, data->session[i].filepath, sizeof(data->session[i].filepath)) == 0)
            {
                index = i;
                break;
            }
        }

        if (index < 0)
        {
            PRINT_ERR("path [%s] is not open", path);
            return 1;   
        }

        // write to the file 

        return 0;
    }

    if(netcmd_str_equal(netcmd_close, cmd, strlen(cmd)))
    {
        // create path/for/file */
        char* token = cmd;
        char *path = strtok(NULL, SEPARATOR);
        PRINT_DBG("[%s][%s]", cmd, path);

        if(strlen(path) > sizeof(data->session[0].filepath)-1 /*EOF*/)
        {
            PRINT_ERR("path [%s] is too big", path);
            return 1;
        }

        // search to see if this session already exists
        for(int i = 0; i < data->session_max; i++)
        {
            if (strncmp(path, data->session[i].filepath, sizeof(data->session[i].filepath)) == 0)
            {
                PRINT_INF("closing path [%s]", path);
                conn->status = NET_SOCK_CLOSED;
                return 0;
            }
        }
    }

    if(netcmd_str_equal(netcmd_exit, cmd, strlen(cmd)))
    {
        PRINT_INF("closing all open files");
        conn->status = NET_SOCK_CLOSED;
        return 0;
    }

    return 1;
}
