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
//#include "mpi.h"
#include "hdf5.h"

int main (int argc, char ** argv) 
{
    char        filename [256];
    int         rank, size, i, j;
//    MPI_Comm    comm = MPI_COMM_WORLD;
    void * data = NULL;
    uint64_t start[2], count[2], bytes_read = 0;
    int ndims;
    hid_t file;
    hid_t dataset;
    hid_t filespace;
    hid_t memspace;

//    MPI_Init (&argc, &argv);
 //   MPI_Comm_rank (comm, &rank);
  //  MPI_Comm_size (comm, &size);

    file = H5Fopen("adios_global.bp", H5F_ACC_RDONLY, H5P_DEFAULT);
    dataset = H5Dopen(file, "index");
    filespace = H5Dget_space(dataset);    /* Get filespace handle first. */
    ndims     = H5Sget_simple_extent_ndims(filespace);

    hsize_t dims[ndims];
    herr_t status_n  = H5Sget_simple_extent_dims(filespace, dims, NULL);
    printf("dataset rank %d, dimensions %lu\n",
	   ndims, (unsigned long)(dims[0])));

    /*
     * Define the memory space to read dataset.
     */
    memspace = H5Screate_simple(ndims,dims,NULL);

    int data_out[dims[0]];
 
    /*
     * Read dataset back and display.
     */
    herr_t status = H5Dread(dataset, H5T_NATIVE_INT, memspace, filespace,
		     H5P_DEFAULT, data_out);
    printf("\n");
    printf("Dataset: \n");
    for (j = 0; j < dims[0]; j++)
    {
	printf("%d ", data_out[j]);
    }     

    printf("\n");
//    MPI_Barrier (comm);

//    MPI_Finalize ();
    return 0;
}
