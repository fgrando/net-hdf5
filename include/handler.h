#ifndef HANLDER_HEADER_
#define HANLDER_HEADER_

#include "net.h"
#include "sink.h"

typedef struct {
   int sock;
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
handler_args_t handler_get_file_sink();
handler_args_t handler_get_hdf5_sink();

#endif