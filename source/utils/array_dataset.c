#include "utils/array_dataset.h"

void add_f32_dataset(hid_t group_id, const char *name,
                     const array_f32_t *data) {
  hsize_t dims[1] = {data->size};
  hid_t type_id = H5T_NATIVE_FLOAT;

  hid_t space_id = H5Screate_simple(1, dims, NULL);
  hid_t dset_id = H5Dcreate(group_id, name, type_id, space_id, H5P_DEFAULT,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data->buffer);
  H5Dclose(dset_id);
  H5Sclose(space_id);
}

void add_d64_dataset(hid_t group_id, const char *name,
                     const array_d64_t *data) {
  hsize_t dims[1] = {data->size};
  hid_t type_id = H5T_NATIVE_DOUBLE;

  hid_t space_id = H5Screate_simple(1, dims, NULL);
  hid_t dset_id = H5Dcreate(group_id, name, type_id, space_id, H5P_DEFAULT,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data->buffer);
  H5Dclose(dset_id);
  H5Sclose(space_id);
}

void add_i32_dataset(hid_t group_id, const char *name,
                     const array_i32_t *data) {
  hsize_t dims[1] = {data->size};
  hid_t type_id = H5T_NATIVE_INT32;

  hid_t space_id = H5Screate_simple(1, dims, NULL);
  hid_t dset_id = H5Dcreate(group_id, name, type_id, space_id, H5P_DEFAULT,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data->buffer);
  H5Dclose(dset_id);
  H5Sclose(space_id);
}

void add_u32_dataset(hid_t group_id, const char *name,
                     const array_u32_t *data) {
  hsize_t dims[1] = {data->size};
  hid_t type_id = H5T_NATIVE_UINT32;

  hid_t space_id = H5Screate_simple(1, dims, NULL);
  hid_t dset_id = H5Dcreate(group_id, name, type_id, space_id, H5P_DEFAULT,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data->buffer);
  H5Dclose(dset_id);
  H5Sclose(space_id);
}

void add_bytes_dataset(hid_t group_id, const char *name,
                       const array_bytes_t *data) {
  hsize_t dims[2] = {data->rows, data->cols};
  hid_t type_id = H5T_NATIVE_UINT8;

  hid_t space_id = H5Screate_simple(2, dims, NULL);
  hid_t dset_id = H5Dcreate(group_id, name, type_id, space_id, H5P_DEFAULT,
                            H5P_DEFAULT, H5P_DEFAULT);
  H5Dwrite(dset_id, type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data->buffer);
  H5Dclose(dset_id);
  H5Sclose(space_id);
}
