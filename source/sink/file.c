#include <stdio.h>
#include <string.h>

#include "sink.h"
#include "print.h"

int file_open(session_t *s, char* path)
{
    strncpy(s->filepath, path, sizeof(s->filepath));
    s->fp = fopen(s->filepath, "w+");
    
    PRINT_DBG("open [%s:%p]", s->filepath, s->fp);
    return (s->fp != NULL) ? 0 : 1;
}

int file_append(session_t *s, char *label, float time, char* buffer, int len)
{
    // do not append if file is not open
    if (s->fp == 0)
    {
        return -1;
    }

    int ret = fprintf(s->fp, "%s %f ", label, time); 
    
    if (ret > 0)
    {
        //fwrite(buffer, 1, len, s->fp);
        for(int i = 0; i < len; i++)
        {
            ret = fprintf(s->fp, "%02x", buffer[i]) > 0 ? 0 : -1;
        }
        
        fprintf(s->fp, "\n");

        s->write_count++;
        if (s->write_count > s->write_max_count)
        {
            fflush(s->fp);
            s->write_count = 0;
        }
    }

    PRINT_DBG("append [%s:%p] %s lc %d", s->filepath, s->fp, label, s->write_count);
    return ret;
}

int file_close(session_t *s)
{
    PRINT_DBG("close [%s:%p]", s->filepath, s->fp);
    int ret = fclose(s->fp); 
    return ret;
}
