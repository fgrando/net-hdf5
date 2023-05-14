#ifndef SINK_HEADER_
#define SINK_HEADER_

#include <stdio.h>

#define HANDLER_MAX_PATH_NAME 255

typedef struct {
    char filepath[HANDLER_MAX_PATH_NAME];
    FILE *fp;
    int write_count;
    int write_max_count;
} session_t;

typedef int (*sink_open_t)(session_t *s, char* path);
typedef int (*sink_append_t)(session_t *s, char *label, float time, char* buffer, int len);
typedef int (*sink_close_t)(session_t *s);

typedef struct {
    sink_open_t open;
    sink_append_t append;
    sink_close_t close;
    session_t session;
} sink_api_t;

extern int file_open(session_t *s, char* path);
extern int file_append(session_t *s, char *label, float time, char* buffer, int len);
extern int file_close(session_t *s);

extern int hdf5_open(session_t *s, char* path);
extern int hdf5_append(session_t *s, char *label, float time, char* buffer, int len);
extern int hdf5_close(session_t *s);

#endif