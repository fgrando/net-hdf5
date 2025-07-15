#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>

#define FILE_NAME "example.h5"
#define NUM_GROUPS 5
#define NUM_DATASETS 2

void write_group(hid_t file_id, int group_idx) {
    char group_name[32];
    sprintf(group_name, "/Group%d", group_idx + 1);

    // Create group
    hid_t group_id = H5Gcreate(file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // --- Dataset 1: Float array
    float float_data[] = {1.1f, 2.2f, 3.3f};
    hsize_t dims1[1] = {3};

    hid_t space1 = H5Screate_simple(1, dims1, NULL);
    hid_t dset1 = H5Dcreate(group_id, "FloatData", H5T_NATIVE_FLOAT, space1, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dset1, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, float_data);
    H5Dclose(dset1);
    H5Sclose(space1);

    // --- Dataset 2: String array (variable length)
    const char *str_data[] = {"Hello", "HDF5", "Group"};
    hsize_t dims2[1] = {3};

    hid_t str_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(str_type, H5T_VARIABLE);  // variable-length strings
    hid_t space2 = H5Screate_simple(1, dims2, NULL);
    hid_t dset2 = H5Dcreate(group_id, "StringData", str_type, space2, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dset2, str_type, H5S_ALL, H5S_ALL, H5P_DEFAULT, str_data);
    H5Dclose(dset2);
    H5Sclose(space2);
    H5Tclose(str_type);


    // Dataset 3: Binary buffer (e.g., 3 elements, each 25 bytes)
    const int e = 3;
    const int buf_size = 25;
    uint8_t binary_data[e][buf_size];

    // Fill with sample values
    for (int i = 0; i < e; i++)
        for (int j = 0; j < buf_size; j++)
            binary_data[i][j] = (uint8_t)(i * 10 + j);

    hsize_t dims3[2] = {e, buf_size};
    hid_t space3 = H5Screate_simple(2, dims3, NULL);

    hid_t dset3 = H5Dcreate(group_id, "BinaryData", H5T_NATIVE_UINT8, space3,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    H5Dwrite(dset3, H5T_NATIVE_UINT8, H5S_ALL, H5S_ALL, H5P_DEFAULT, binary_data);
    H5Dclose(dset3);
    H5Sclose(space3);

    H5Gclose(group_id);
}

int main() {
    // Create a new HDF5 file
    hid_t file_id = H5Fcreate(FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // Create multiple groups and datasets
    for (int i = 0; i < NUM_GROUPS; i++) {
        write_group(file_id, i);
    }

    // Close the file
    H5Fclose(file_id);
    printf("HDF5 file '%s' created with %d groups.\n", FILE_NAME, NUM_GROUPS);
    return 0;
}
