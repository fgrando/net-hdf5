#include "netcmd.h"
#include "string.h"
#include "print.h"

#define NETCMD_NACK   "nack"
#define NETCMD_ACK    "ack"
#define NETCMD_ABORT  "abort"
#define NETCMD_CREATE "create"
#define NETCMD_APPEND "append"
#define NETCMD_CLOSE  "close"


int netcmd_to_str(netcmd_e cmd, char* buffer, int len)
{
    int ret = 0;
    memset(buffer, 0, len);
    switch (cmd)
    {
    case netcmd_nack:
        strncat(buffer, NETCMD_NACK, len);
        ret = strlen(buffer);
        break;
    case netcmd_ack:
        strncat(buffer, NETCMD_ACK, len);
        ret = strlen(buffer);
        break;
    case netcmd_abort:
        strncat(buffer, NETCMD_ABORT, len);
        ret = strlen(buffer);
        break;
    case netcmd_create:
        strncat(buffer, NETCMD_CREATE, len);
        ret = strlen(buffer);
        break;
    case netcmd_append:
        strncat(buffer, NETCMD_APPEND, len);
        ret = strlen(buffer);
        break;
    case netcmd_close:
        strncat(buffer, NETCMD_CLOSE, len);
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
    if(cmd == netcmd_nack)   return strncmp(str_cmd, NETCMD_NACK,   len)==0 ? 1 : 0;
    if(cmd == netcmd_ack)    return strncmp(str_cmd, NETCMD_ACK,    len)==0 ? 1 : 0;
    if(cmd == netcmd_abort)  return strncmp(str_cmd, NETCMD_ABORT,  len)==0 ? 1 : 0;
    if(cmd == netcmd_create) return strncmp(str_cmd, NETCMD_CREATE, len)==0 ? 1 : 0;
    if(cmd == netcmd_append) return strncmp(str_cmd, NETCMD_APPEND, len)==0 ? 1 : 0;
    if(cmd == netcmd_close)  return strncmp(str_cmd, NETCMD_CLOSE,  len)==0 ? 1 : 0;
    return 0;
}