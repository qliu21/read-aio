/* 
 * ADIOS is freely available under the terms of the BSD license described
 * in the COPYING file in the top level directory of this source distribution.
 *
 * Copyright (c) 2008 - 2009.  UT-BATTELLE, LLC. All rights reserved.
 */

/* ADIOS C Example: read global arrays from a BP file
 *
 * This code is using the generic read API, which can read in
 * arbitrary slices of an array and thus we can read in an array
 * on arbitrary number of processes (provided our code is smart 
 * enough to do the domain decomposition).
 *
 * Run this example after adios_global, which generates 
 * adios_global.bp. Run this example on equal or less 
 * number of processes since we decompose only on one 
 * dimension of the global array here. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hdf5.h"

int main (int argc, char ** argv) 
{
    char        filename [256];
    int         rank, size, i, j;
    uint64_t start[2], count[2], bytes_read = 0;
    int ndims, nsf;
    hid_t file;
    hid_t dataset;
    hid_t filespace;
    hid_t memspace;

    int data_out[size];

    struct timeval t1;
    gettimeofday (&t1, NULL);

    hid_t fid = H5Fcreate ("write_test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    hid_t h5_mtype_id = H5Tcopy(H5T_NATIVE_INT);

    hsize_t h5_mdims[1];
    h5_mdims[0] = 48;
    hid_t h5_mdataspace_id = H5Screate_simple (1, h5_mdims, NULL);
    hid_t h5_mdataset_id = H5Dcreate (fid
                                     ,"index"
                                     ,h5_mtype_id
                                     ,h5_mdataspace_id
                                     ,H5P_DEFAULT);

    int data[48];
    herr_t status = H5Dwrite (h5_mdataset_id
                          ,h5_mtype_id
                          ,H5S_ALL
                          ,H5S_ALL
                          ,H5P_DEFAULT
                          ,data
                          );

    struct timeval t2;
    gettimeofday (&t2, NULL);

    printf ("write time = %f\n", t2.tv_sec + t2.tv_usec/1000000.0 - t1.tv_sec - t1.tv_usec/1000000.0); 
    return 0;
}
