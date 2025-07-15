#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "hdf5.h"
#include "utils/array.h"
#include "utils/array_dataset.h"
#include "utils/print.h"

#define FILE_NAME "example.h5"

void add_int_attribute(hid_t loc_id, const char *attr_name, int value) {
  hid_t attr_space = H5Screate(H5S_SCALAR);
  hid_t attr_id = H5Acreate(loc_id, attr_name, H5T_NATIVE_INT, attr_space,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attr_id, H5T_NATIVE_INT, &value);
  H5Aclose(attr_id);
  H5Sclose(attr_space);
}

void add_string_attribute(hid_t loc_id, const char *attr_name,
                          const char *value) {
  hid_t attr_space = H5Screate(H5S_SCALAR);
  hid_t str_type = H5Tcopy(H5T_C_S1);
  H5Tset_size(str_type, H5T_VARIABLE);
  hid_t attr_id = H5Acreate(loc_id, attr_name, str_type, attr_space,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Awrite(attr_id, str_type, &value);
  H5Aclose(attr_id);
  H5Sclose(attr_space);
  H5Tclose(str_type);
}

int main() {

  array_d64_t time;
  array_d64_init(&time, 1000);

  array_f32_t weights;
  array_f32_init(&weights, 1000);

  array_d64_t biases;
  array_d64_init(&biases, 1000);

  array_i32_t indices;
  array_i32_init(&indices, 1000);

  array_u32_t labels;
  array_u32_init(&labels, 1000);

  array_bytes_t pictures;
  array_bytes_init(&pictures, 1000,
                   64); // Example: 1000 images of size 64 bytes each

  // Initialize weights with some values
  for (int i = 0; i < weights.size; i++) {
    // put random values:
    weights.buffer[i] =
        (float)rand() / RAND_MAX;       // Random float between 0 and 1
    labels.buffer[i] = i * 2;           // Example label
    indices.buffer[i] = i;              // Example index
    biases.buffer[i] = (double)rand();  // Example bias
    time.buffer[i] = (double)i * 0.001; // Example time

    for (int j = 0; j < pictures.cols; j++) {
      pictures.buffer[i * pictures.cols + j] =
          (uint8_t)(rand() % 256); // Random byte
    }
  }

  // Create a new HDF5 file
  hid_t file_id = H5Fcreate(FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  const char *author = "John Doe";
  const char *description =
      "This file contains 5 groups with mixed-type datasets";
  int version = 2;

  add_string_attribute(file_id, "Author", author);
  add_string_attribute(file_id, "Description", description);
  add_int_attribute(file_id, "Version", version);

  hid_t group_id;
  char group_name[32];
  sprintf(group_name, "/weights");
  group_id =
      H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  add_d64_dataset(group_id, "time", &time);
  add_f32_dataset(group_id, "weights", &weights);

  sprintf(group_name, "/biases");
  group_id =
      H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  add_d64_dataset(group_id, "time", &time);
  add_d64_dataset(group_id, "biases", &biases);

  sprintf(group_name, "/indices");
  group_id =
      H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  add_d64_dataset(group_id, "time", &time);
  add_i32_dataset(group_id, "indices", &indices);

  sprintf(group_name, "/labels");
  group_id =
      H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  add_d64_dataset(group_id, "time", &time);
  add_u32_dataset(group_id, "labels", &labels);

  sprintf(group_name, "/pictures");
  group_id =
      H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  add_d64_dataset(group_id, "time", &time);
  add_bytes_dataset(group_id, "pictures", &pictures);

  // Close the file
  H5Fclose(file_id);
  printf("HDF5 file '%s' created\n", FILE_NAME);

  array_f32_free(&weights);
  array_d64_free(&time);
  array_d64_free(&biases);
  array_i32_free(&indices);
  array_u32_free(&labels);
  array_bytes_free(&pictures);

  return 0;
}
