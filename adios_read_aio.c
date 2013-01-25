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
#include "mpi.h"
#include "hdf5.h"

int main (int argc, char ** argv) 
{
    char        filename [256];
    int         rank, size, i, j;
    MPI_Comm    comm = MPI_COMM_WORLD;
    uint64_t start[2], count[2], bytes_read = 0;
    int ndims, nsf;
    hid_t file;
    hid_t dataset;
    hid_t filespace;
    hid_t memspace;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (comm, &rank);
    MPI_Comm_size (comm, &size);

    int data_out[size];

    MPI_Barrier (comm);
    struct timeval t1;
    gettimeofday (&t1, NULL);

    strcpy (filename, "adios_global.bp");
    {
        file = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT);
        dataset = H5Dopen(file, "index");
        filespace = H5Dget_space(dataset);    /* Get filespace handle first. */
        ndims     = H5Sget_simple_extent_ndims(filespace);

        hsize_t dims[ndims];
        herr_t status_n  = H5Sget_simple_extent_dims(filespace, dims, NULL);
/*
        printf("dataset rank %d, dimensions %lu\n",
	       ndims, (unsigned long)(dims[0]));
*/
       if (dims[0] != size)
       {
           printf ("can only support same # of reader as writers\n");
           exit (0);
       }
       /*
        * Define the memory space to read dataset.
        */
        memspace = H5Screate_simple(ndims,dims,NULL);

        /*
         * Read dataset back and display.
         */
        herr_t status = H5Dread(dataset, H5T_NATIVE_INT, memspace, filespace,
	          	     H5P_DEFAULT, data_out);
    }

    int temp_index[size];
    int temp;
    memcpy (temp_index, data_out, 4 * size);
  
    nsf = 0;
    for (i = 0; i < size - 1; i++)
    {
        if (temp_index[i] > temp_index[i + 1])
        {
            temp = temp_index[i];
            temp_index[i] = temp_index[i + 1];
            temp_index[i + 1] = temp;
        }
    }

    nsf = temp_index[size - 1] + 1;
printf ("nsf = %d\n", nsf); 
    // data is in f_idx subfile.
    int f_idx = data_out[rank];
    char temp_string[100], * fname;
    strcpy (temp_string, filename);
    fname = strtok (temp_string, ".");

    sprintf (temp_string
            ,"%s_%d.bp"
            ,fname
            ,f_idx
            );
    file = H5Fopen(temp_string, H5F_ACC_RDONLY, H5P_DEFAULT);
    sprintf (temp_string, "temperature_%d", rank);
    dataset = H5Dopen(file, temp_string);
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

    double data[dims[1]];
    /*
     * Read dataset back and display.
     */
    herr_t status = H5Dread(dataset, H5T_NATIVE_DOUBLE, memspace, filespace,
                         H5P_DEFAULT, data);

/*
    printf("\n");
    printf("Dataset: \n");
    for (j = 0; j < dims[1]; j++)
    {
        printf("%f ", data[j]);
    }

    printf("\n");
*/
    MPI_Barrier (comm);
    struct timeval t2;
    gettimeofday (&t2, NULL);

    if (rank == 0)
        printf("read time: %.6lf\n", t2.tv_sec + t2.tv_usec/1000000.0 - t1.tv_sec - t1.tv_usec/1000000.0); 

    MPI_Finalize ();
    return 0;
}
