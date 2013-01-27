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

        file = H5Fopen("write_test.h5", H5F_ACC_RDONLY, H5P_DEFAULT);
        dataset = H5Dopen(file, "index");
        filespace = H5Dget_space(dataset);    /* Get filespace handle first. */
        ndims     = H5Sget_simple_extent_ndims(filespace);

        hsize_t dims[ndims];
        herr_t status_n  = H5Sget_simple_extent_dims(filespace, dims, NULL);
/*
    printf("dataset(temperature) rank %d, dimensions %lux%lu\n",
               ndims, (unsigned long)(dims[0]), (unsigned long)(dims[1]));
*/
    /*
     * Define the memory space to read dataset.
     */
        memspace = H5Screate_simple(ndims,dims,NULL);

        double data[dims[0]];
    /*
     * Read dataset back and display.
     */
        herr_t status = H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, filespace,
                         H5P_DEFAULT, data);

    struct timeval t2;
    gettimeofday (&t2, NULL);

    printf ("read time = %f\n", t2.tv_sec + t2.tv_usec/1000000.0 - t1.tv_sec - t1.tv_usec/1000000.0); 
    return 0;
}
