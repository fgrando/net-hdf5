#ifndef HANLDER_HEADER_
#define HANLDER_HEADER_

#include "net.h"
#include "sink.h"



typedef struct
{
    char filepath[2];
    char last_cmd[2];
    char last_result[2];
} fd_session_t;


typedef struct
{
    char name[20];
    sink_api_t sink;
    int *keep_running;  // abort program
} handler_args_t;

void handler_client(net_client_handler_args_t *args);
void handler_set_file_sink(handler_args_t *args);
void handler_set_hdf5_sink(handler_args_t *args);

int create_session(handler_args_t* args);
int close_session(handler_args_t* args);

#endif