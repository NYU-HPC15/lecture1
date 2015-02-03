/* multiply vector components, write into a vector,
 *  and compute the inner product
 * Need to compile this with an mpi-capable compiler (depending on your * architecture, mpicc or similar) */
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <mpi.h>


int main (int argc, char **argv)
{
  int mpisize, mpirank;
  long  i, n;
  double *a, *b, *c;
  double my_prod, prod;
  long p, passes;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &mpisize);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);

  if (argc != 3) {
    fprintf(stderr, "Function needs vector size and number of passes as input arguments!\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  n = atol(argv[1]);
  passes = atol(argv[2]);

  if (n % mpisize != 0) {
    fprintf(stderr, "Vector size not divisible!\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  printf("rank %d/%d reporting for duty\n", mpirank, mpisize);

  long my_n = n / mpisize;
  long offset = mpirank * my_n;

  a = (double *) malloc(sizeof(double) * my_n);
  b = (double *) malloc(sizeof(double) * my_n);
  c = (double *) malloc(sizeof(double) * my_n);

  /* fill vectors */
  for (i = 0; i < my_n; ++i) {
    a[i] = 1 + offset + i;
    b[i] = 2./(offset + i + 1);
  }

  timestamp_type time1, time2;
  get_timestamp(&time1);

  for (p = 0; p < passes; ++p)
    {
      for (i = 0; i < my_n; ++i) {
	c[i] = a[i] * b[i];
      }

      my_prod = 0.0;
      for (i = 0; i < my_n; ++i) {
      	my_prod += c[i];
      }
      MPI_Reduce(&my_prod, &prod, 1, MPI_DOUBLE, MPI_SUM,
		 0, MPI_COMM_WORLD);
    }

  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  if (mpirank == 0) {
    printf("Time elapsed is %f seconds.\n", elapsed);
    printf("Inner product is %f.\n", prod);

    printf("%f GB/s\n", 4*n*sizeof(double)*passes/1e9/elapsed);
    printf("%f GFlops/s\n", 2*n*passes/1e9/elapsed);
  }
  free(a);
  free(b);
  free(c);

  MPI_Finalize();
  return 0;
}
