#ifndef NETCMD_HEADER_INCLUDED_
#define NETCMD_HEADER_INCLUDED_

typedef enum{
#include "../commands.dat"
netcmd_total_elements
} netcmd_e;

int netcmd_to_str(netcmd_e cmd, char* buffer, int len);
int netcmd_str_equal(netcmd_e cmd, char* str_cmd);

#endif