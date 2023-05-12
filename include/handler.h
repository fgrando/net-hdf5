#ifndef HANLDER_HEADER_
#define HANLDER_HEADER_

#include "net.h"
#include "sink.h"

#define HANDLER_MAX_PATH_NAME 254

typedef struct
{
    char filepath[HANDLER_MAX_PATH_NAME];
    char last_cmd[HANDLER_MAX_PATH_NAME];
    char last_result[HANDLER_MAX_PATH_NAME];
} fd_session_t;

typedef struct
{
    char name[20];
    sink_api_t sink;
    int *keep_running;  // abort program
    fd_session_t session[10];      // handle up to 10 files
} handler_args_t;

void handler_client(net_client_handler_args_t *args);
void handler_set_file_sink(handler_args_t *args);
void handler_set_hdf5_sink(handler_args_t *args);

#endif