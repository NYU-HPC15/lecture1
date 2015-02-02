#include <stdio.h>
#include <stdlib.h>
/* timing in util.h requires -lrt flag to compile */
#include "util.h"

int main (int argc, char **argv)
{
  long i, n;
  double s;

  if (argc != 2)
    {
      fprintf(stderr, "need an argument (number of summands)\n");
      abort();
    }

  n = atol(argv[1]);
  s = 0.0;

  timestamp_type time1, time2;
  get_timestamp(&time1);

  for (i = 1; i < n; ++i) {
    s = s + 1.0/i;
  }

  get_timestamp(&time2);
  double elapsed = timestamp_diff_in_seconds(time1,time2);

  printf("The sum of 1/n over the first %ld numbers is %f !\n", n, s);
  printf("Time elapsed is %f seconds.\n", elapsed);

  return 0;
}
