#include <stdio.h>

typedef struct {
    FILE *fp;
} session_t;

typedef void (*sink_open_t)(session_t *s, char* path);
typedef void (*sink_append_t)(session_t *s, char *label, float time, char* buffer, int len);
typedef void (*sink_close_t)(session_t *s);

typedef struct {
    sink_open_t open;
    sink_append_t append;
    sink_close_t close;
    session_t session;
} sink_api_t;


extern sink_api_t SINK_API_FILE;
extern sink_api_t SINK_API_HDF5;
