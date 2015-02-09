# lecture1

For the OpenMP example, you need to use -fopenmp to link against the
OpenMP libs, e.g.:

gcc inner-omp.c -a inner-omp -fopenmp

Make sure your compiler handels OpenMP, I use gcc4.9 on my Mac.


