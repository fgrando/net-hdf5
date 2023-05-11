#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "persistance.h"
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

int parse_max_clients(char* str)
{
    char* token = str;
    token = strtok(NULL, SEPARATOR);
    return (atoi(token));
}

int parse_config(char *filepath, persistance_cfg_t *cfg)
{
    memset(cfg, 0, sizeof(persistance_cfg_t)); // clear all inputs
    cfg->version = 1; // FIXME

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
            cfg->port = parse_listen_port(line);
            PRINT_INF("%s...%d", param, cfg->port);
        }
        else if (0 == strcmp(param, "max_clients"))
        {
            cfg->max_clients = parse_max_clients(line);
            PRINT_INF("%s...%d", param, cfg->max_clients);
        }
        else
        {
            // not parsed
            PRINT_INF("line (%s) parsed param [%s]", line, param);
        }

    }

    fclose(cfgfile);
}