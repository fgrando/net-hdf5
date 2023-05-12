#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "cfg_file.h"
#include "print.h"


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

int cfg_file_to_str(cfg_file_t *data, char *buffer, int len)
{
    return 0;
}

int cfg_file_save(char const * const filepath, cfg_file_t const * const data)
{
    return -1;
}

int cfg_file_load(char const * const filepath, cfg_file_t * data)
{
    memset(data, 0, sizeof(cfg_file_t)); // clear all inputs
    data->version = 1; // FIXME

    char line[CFG_FILE_MAX_LINE_CHARS] = {0};
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
            data->port = parse_listen_port(line);
            PRINT_INF("%s...%d", param, data->port);
        }
        else if (0 == strcmp(param, "max_clients"))
        {
            data->max_clients = parse_max_clients(line);
            PRINT_INF("%s...%d", param, data->max_clients);
        }
        else
        {
            // not parsed
            PRINT_INF("line (%s) parsed param [%s]", line, param);
        }

    }

    fclose(cfgfile);
}