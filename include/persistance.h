#ifndef PARSE_HEADER_INCLUDED_
#define PARSE_HEADER_INCLUDED_

typedef struct {
    int version;
    int port;
    int max_clients;
} persistance_cfg_t;

int parse_config(char *filepath, persistance_cfg_t *cfg);

#endif