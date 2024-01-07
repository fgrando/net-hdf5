#ifndef INTERFACE_H
#define INTERFACE_H

#include "msg1_colors.h"


int msg1_colors_load_net_raw(const char* const buffer, int size, msg1_colors_data_t* data);
int msg1_colors_dump_net_raw(const msg1_colors_data_t* const data, char* buffer, int size);

#endif
