/* multiply vector components, write into a vector,
 *  and compute the inner product  */
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <omp.h>

int main (int argc, char **argv)
{
  long  i, n;
  double *a, *b, *c;
  double prod;
  long p, passes;

  if (argc != 3) {
    fprintf(stderr, "Function needs vector size and number of passes as input arguments!\n");
    abort();
  }

  n = atoi(argv[1]);
  passes = atoi(argv[2]);

  a = (double *) malloc(sizeof(double) * n);
  b = (double *) malloc(sizeof(double) * n);
  c = (double *) malloc(sizeof(double) * n);

  /* fill vectors */
  for (i = 0; i < n; ++i) {
    a[i] = i + 1;
    b[i] = 2./(i + 1);
  }

#pragma omp parallel
  {
    printf("Hello hello. I'm thread %d of %d\n",
	   omp_get_thread_num(),
	   omp_get_num_threads());
  }
  timestamp_type time1, time2;
  get_timestamp(&time1);

  for (p = 0; p < passes; ++p)
    {
#pragma omp parallel for default(none) shared(n,a,b,c)
      for (i = 0; i < n; ++i) {
	c[i] = a[i] * b[i];
      }
      prod = 0.0;
#pragma omp parallel for reduction(+:prod)
      for (i = 0; i < n; ++i) {
      	prod += c[i];
      }
    }

  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);
  printf("Time elapsed is %f seconds.\n", elapsed);
  printf("Inner product is %f.\n", prod);

  printf("%f GB/s\n", 4*n*sizeof(double)*passes/1e9/elapsed);
  printf("%f GFlops/s\n", 2*n*passes/1e9/elapsed);

  free(a);
  free(b);
  free(c);
  return 0;
}
