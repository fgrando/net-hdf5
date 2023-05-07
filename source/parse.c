#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "print.h"

#define MAX_CFG_PARAM 10
#define MAX_CFG_LINE (MAX_CFG_PARAM + 80)

const char SEPARATOR[2] = "=";

int parse_listen_port(char* str)
{
    char* token = str;
    token = strtok(NULL, SEPARATOR);
    return (atoi(token));
}

int parse_config(char *filepath, int* port)
{
    char line[MAX_CFG_LINE] = {0};
    FILE *cfgfile = 0;

    cfgfile = fopen(filepath, "r");
    if(cfgfile == NULL)
    {
        PRINT_ERR("Failed to read config from %s", filepath);
        return 1;
    }

    PRINT_INF("parsing config from: %s", filepath);

    while(fgets(line, sizeof(line), cfgfile)){

        char *param = strtok(line, SEPARATOR);

        if (0 == strcmp(param, "listen_port"))
        {
            *port = parse_listen_port(line);
            PRINT_INF("%s...%d", param, *port);
        }
        else
        {
            // not parsed
            PRINT_INF("line (%s) parsed param [%s]", line, param);
        }

    }

    fclose(cfgfile);
}