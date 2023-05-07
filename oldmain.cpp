#include "hdf5.h"

#define H5FILE_NAME        "SDS.h5"
#define DATASETNAME "IntArray"
#define NX     3                      /* dataset dimensions */
#define NY     6
#define RANK   2

int
h5me (void)
{
    hid_t       file, dataset;         /* file and dataset handles */
    hid_t       datatype, dataspace;   /* handles */
    hsize_t     dimsf[3];              /* dataset dimensions */
    herr_t      status;
    int         data[NX][NY];          /* data to write */
    int         i, j;

    /*
     * Data  and output buffer initialization.
     */
    for(j = 0; j < NX; j++)
	for(i = 0; i < NY; i++)
	    data[j][i] = i + j;
    /*
     * 0 1 2 3 4 5
     * 1 2 3 4 5 6
     * 2 3 4 5 6 7
     * 3 4 5 6 7 8
     * 4 5 6 7 8 9
     */

    /*
     * Create a new file using H5F_ACC_TRUNC access,
     * default file creation properties, and default file
     * access properties.
     */
    file = H5Fcreate(H5FILE_NAME, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Describe the size of the array and create the data space for fixed
     * size dataset.
     */
    dimsf[0] = NX;
    dimsf[1] = NY;
    dataspace = H5Screate_simple(RANK, dimsf, NULL);

    /*
     * Define datatype for the data in the file.
     * We will store little endian INT numbers.
     */
    datatype = H5Tcopy(H5T_NATIVE_INT);
    status = H5Tset_order(datatype, H5T_ORDER_BE);

    /*
     * Create a new dataset within the file using defined dataspace and
     * datatype and default dataset creation properties.
     */
    dataset = H5Dcreate2(file, DATASETNAME, datatype, dataspace,
			H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    /*
     * Write the data to the dataset using default transfer properties.
     */
    status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);


    dataset = H5Dcreate2(file, "time", datatype, dataspace,
			H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite(dataset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);

    /*
     * Close/release resources.
     */
    H5Sclose(dataspace);
    H5Tclose(datatype);
    H5Dclose(dataset);
    H5Fclose(file);

    return 0;
}





int main()
{

   h5me();

   char* peer = "127.0.0.1";

   NetLibSetup setup = { 0 };
   setup.port = 8000;
   setup.isServer = 1;
   setup.type = NetLibSockTcp;
   setup.ipAddr = peer;

   puts("build with: gcc -g help.c -lws2_32");

   int ret = NetLibInit(&setup);
   DEBUG_PRINT("ret %d", ret);

   ret = NetLibOpen(&setup);
   DEBUG_PRINT("ret %d", ret);


    do {

         char buff[1000] = { 0 };
         ret = NetLibRecv(buff, sizeof(buff));
         DEBUG_PRINT("ret %d %s", ret, buff);

        if (ret > 0) {

            ret = NetLibSend(buff, ret);
            DEBUG_PRINT("ret %d", ret);

            if (ret == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());

               ret = NetLibClose(&setup);
               DEBUG_PRINT("ret %d", ret);
               ret = NetLibFinish(&setup);
               DEBUG_PRINT("ret %d", ret);

                return 1;
            }
            printf("Bytes sent: %d\n", ret);
        }
        else if (ret == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());

            ret = NetLibClose(&setup);
            DEBUG_PRINT("ret %d", ret);
            ret = NetLibFinish(&setup);
            DEBUG_PRINT("ret %d", ret);

            return 1;
        }

    } while (ret > 0);





   ret = NetLibClose(&setup);
   DEBUG_PRINT("ret %d", ret);

   ret = NetLibFinish(&setup);
   DEBUG_PRINT("ret %d", ret);

    return 0;
}