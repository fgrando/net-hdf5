#include "sink.h"
#include <stdio.h>

void file_open(session_t *s, char* path)
{
    s->fp = fopen(path, "w+");
}

void file_append(session_t *s, char *label, float time, char* buffer, int len)
{
    fprintf(s->fp, "%s: %f", label, time);
    fwrite(buffer, 1, len, s->fp);
    fprintf(s->fp, "\n");
}

void file_close(session_t *s)
{
    fclose(s->fp);
}
