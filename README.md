# HPCProject-ParallelProgramming

Repository for the High Performance Computing personal project A.Y. 2023/2024, regarding the use of various techniques for parallel programming.

In this repository, you'll find:
- In the "BigData" directory, two exercises based on a dataset of World Cup players, performed on a Docker cluster with Hadoop and Spark (Java). You will also find the final report of the entire project, in which this part is illustrated in the second chapter;
- In the "OMP_MPI_CUDA" directory, the parallelization of the Levenshtein edit distance algorithm is implemented (with approximation). Specifically, the source code for the sequential version of the algorithm, a version leveraging the processor with OMP+MPI (C), and a version utilizing the processor and GPU with OMP+CUDA are provided (C). Everything is compiled and executed using a "makefile", and additionally, all tables and data resulting from executions performed with various input sizes (string lengths for which to calculate the distance) are included. Also included is the final report of the entire project, in which this part is illustrated in the first chapter.
