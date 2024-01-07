#include <stdio.h>
#include <stdint.h>

#include "msg1_colors/interface.h"

int main()
{
    uint8_t rawbuffer[2000];
    msg1_colors_data_t msg1;

    int errors = msg1_colors_load_net_raw(rawbuffer, sizeof(rawbuffer), &msg1);

    

    puts("formats");
    return 0;
}