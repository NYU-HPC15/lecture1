# lecture1

Example files for the multiplication of the components of two vectors,
and the computation of the inner product between two vectors.
Contains serial version, as well as OpenMP and MPI versions.


For the OpenMP example, you need to use -fopenmp to link against the
OpenMP libs, e.g.:

gcc inner-omp.c -a inner-omp -fopenmp

Make sure your compiler handels OpenMP, I use gcc4.9 on my Mac.


