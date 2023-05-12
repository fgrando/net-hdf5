#include "netcmd.h"
#include "string.h"
#include "print.h"

#define NETCMD_NACK "netcmd_nack"
#define NETCMD_ACK "netcmd_ack"
#define NETCMD_ABORT "netcmd_abort"
#define NETCMD_CREATE "netcmd_create"
#define NETCMD_APPEND "netcmd_append"
#define NETCMD_CLOSE "netcmd_close"


int netcmd_to_str(netcmd_e cmd, char* buffer, int len)
{
    int ret = 0;
    memset(buffer, 0, len);
    switch (cmd)
    {
    case netcmd_nack:
        strncat(buffer, len, NETCMD_NACK);
        ret = strlen(buffer);
        break;
    case netcmd_ack:
        strncat(buffer, len, NETCMD_ACK);
        ret = strlen(buffer);
        break;
    case netcmd_abort:
        strncat(buffer, len, NETCMD_ABORT);
        ret = strlen(buffer);
        break;
    case netcmd_create:
        strncat(buffer, len, NETCMD_CREATE);
        ret = strlen(buffer);
        break;
    case netcmd_append:
        strncat(buffer, len, NETCMD_APPEND);
        ret = strlen(buffer);
        break;
    case netcmd_close:
        strncat(buffer, len, NETCMD_CLOSE);
        ret = strlen(buffer);
        break;
    default:
        PRINT_ERR("command %d is unknown", cmd);
        break;
    }

    return ret;
}

int netcmd_str_equal(netcmd_e cmd, char* str_cmd, int len)
{
    if(cmd == netcmd_nack)   return strncmp(str_cmd, NETCMD_NACK,   len) ? 1 : 0;
    if(cmd == netcmd_ack)    return strncmp(str_cmd, NETCMD_ACK,    len) ? 1 : 0;
    if(cmd == netcmd_abort)  return strncmp(str_cmd, NETCMD_ABORT,  len) ? 1 : 0;
    if(cmd == netcmd_create) return strncmp(str_cmd, NETCMD_CREATE, len) ? 1 : 0;
    if(cmd == netcmd_append) return strncmp(str_cmd, NETCMD_APPEND, len) ? 1 : 0;
    if(cmd == netcmd_close)  return strncmp(str_cmd, NETCMD_CLOSE,  len) ? 1 : 0;
    return 0;
}