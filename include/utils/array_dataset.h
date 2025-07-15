#ifndef C0EC582F_5968_435B_8DA0_902C534ADD79
#define C0EC582F_5968_435B_8DA0_902C534ADD79

#include <hdf5.h>
#include "utils/array.h"

void add_f32_dataset(hid_t group_id, const char* name, const array_f32_t *data);
void add_d64_dataset(hid_t group_id, const char* name, const array_d64_t *data);
void add_i32_dataset(hid_t group_id, const char* name, const array_i32_t *data);
void add_u32_dataset(hid_t group_id, const char* name, const array_u32_t *data);
void add_bytes_dataset(hid_t group_id, const char* name, const array_bytes_t *data);

#endif /* C0EC582F_5968_435B_8DA0_902C534ADD79 */
