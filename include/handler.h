#ifndef HANLDER_HEADER_
#define HANLDER_HEADER_

#include "net.h"
#include "sink.h"

typedef struct {
   net_client_handler_args_t net;
   char name[20];
   sink_api_t sink;
} handler_args_t;

typedef enum session_state{
    INVALID,
    OPEN,
    APPEND,
    CLOSE
} e_session_state;


void handler_client(handler_args_t *args);
void handler_set_file_sink(handler_args_t *args);
void handler_set_hdf5_sink(handler_args_t *args);

#endif