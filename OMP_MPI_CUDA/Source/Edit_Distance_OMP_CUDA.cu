/*
 * Course: High Performance Computing 2023/2024
 *
 * Lecturer: Francesco Moscato	fmoscato@unisa.it
 *
 * Author:
 * Ciaravola Giosuè		0622702177		g.ciaravola3@studenti.unisa.it
 *
 * Copyright (C) 2024 - All Rights Reserved
 *
 * This file is part of ProjectHPC.
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation, either version
 * 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with ContestOMP.
 * If not, see <https://www.gnu.org/licenses/gpl-3.0.html>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define CUDA_CHECK(X) {\
 cudaError_t _m_cudaStat = X;\
 if(cudaSuccess != _m_cudaStat) {\
    fprintf(stderr,"\nCUDA_ERROR: %s in file %s line %d\n",\
    cudaGetErrorString(_m_cudaStat), __FILE__, __LINE__);\
    exit(1);\
 } }

/////////////////////////////////////////////////////////RANDOM STRING GENERATION/////////////////////////////////////////////////////////////////

/**
 * @brief Generates a random string of specified length using the given seed.
 *
 * @param n_characters The desired length of the generated string.
 * @param seed         The seed value for the random number generator.
 * 
 * @return             A dynamically allocated char array (string) containing
 *                     random characters. It is the responsibility of the caller
 *                     to free the allocated memory using free() when done using
 *                     the generated string.
 *                     Returns NULL in case of memory allocation failure.
 */
char* generateRandomString(int n_characters, int seed) {
    // Define the character alphabet for generating random strings
    static const char alphabet[] = "abcde fghijklmn opqrst uvwxyz";
    
    // Calculate the size of the alphabet (excluding the null terminator)
    int alphabetSize = sizeof(alphabet) - 1;

    // Allocate memory for the generated string, including space for the null terminator
    char* generatedString = (char*)malloc((n_characters + 1) * sizeof(char));

    // Check for memory allocation failure
    if (generatedString == NULL) {
        fprintf(stderr, "ERROR in memory allocation.\n");
        exit(EXIT_FAILURE);
    }

    // Seed the random number generator
    srand(seed);

    // Generate random characters based on the alphabet and fill the string
    for (int i = 0; i < n_characters; ++i)
        generatedString[i] = alphabet[rand() % alphabetSize];

    // Add the null terminator at the end of the generated string
    generatedString[n_characters] = '\0';

    // Return the generated random string
    return generatedString;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief CUDA function to compute the length of a null-terminated string.
 *
 * This function calculates the length of the input null-terminated string.
 *
 * @param str Pointer to the null-terminated input string.
 * @return    Length of the input string (excluding the null terminator).
 */
__host__ __device__ int cuda_strlen(const char* str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

/**
 * @brief CUDA function to copy a null-terminated string up to a specified size.
 *
 * This function copies characters from the source string to the destination
 * string up to the specified size. If the source string is shorter than the
 * specified size, the destination string is null-terminated.
 *
 * @param dest Pointer to the destination string.
 * @param src  Pointer to the source null-terminated string.
 * @param size Maximum number of characters to copy.
 */
__host__ __device__ void cuda_strcpy(char* dest, const char* src, int size) {
    for (int i = 0; i < size; ++i) {
        dest[i] = src[i];
        if (src[i] == '\0') {
            break;
        }
    }
}

/**
 * @brief CUDA function to find the minimum of two floating-point numbers.
 *
 * This function returns the minimum value between the two input floating-point numbers.
 *
 * @param a First floating-point number.
 * @param b Second floating-point number.
 * @return  The minimum of the two input floating-point numbers.
 */
__host__ __device__ float cuda_fmin(float a, float b) {
    return (a < b) ? a : b;
}

/**
 * @brief Calculates the partitioned substring based on the given parameters.
 *
 * This function takes the entire string, the index of the partition, and the total number
 * of partitions as input parameters. It calculates the appropriate substring for the specified
 * partition, attempting to distribute the elements as evenly as possible, and returns it as a
 * dynamically allocated char array.
 *
 * @param str   The entire string to be partitioned.
 * @param id    The index of the partition.
 * @param n_id  The total number of partitions.
 *
 * @return      A dynamically allocated char array containing the partitioned substring.
 *              It is the responsibility of the caller to free the allocated memory using free().
 */
__host__ __device__ char* partitioning(const char* str, int id, int n_id) {

    int len = cuda_strlen(str);

    // Calculate the lengths and starting points of local sections of work
    int local_len = len / n_id; 
    int remaining = len % n_id; 
    int local_start = id * local_len + cuda_fmin(id, remaining); 
    local_len += (id < remaining) ? 1 : 0; 

    // Allocate memory for the local substring
    char* local_str = (char*)malloc((local_len + 1) * sizeof(char));

    // Copy the partitioned substring into the allocated memory
    cuda_strcpy(local_str, str + local_start, local_len);
    local_str[local_len] = '\0';

    return local_str;
}

/**
 * @brief Calculates the Levenshtein distance between two strings.
 *
 * The Levenshtein distance is the minimum number of single-character edits 
 * (insertions, deletions, or substitutions) required to change one word into another.
 *
 * @param str1 The first input string.
 * @param str2 The second input string.
 * 
 * @return     The Levenshtein distance between the two input strings.
 *             Returns -1 if memory allocation fails during distance matrix creation.
 */
__host__ __device__ int levenshteinDistance(const char* str1, const char* str2) {
    // Lengths of input strings plus one for the null terminator
    int m = cuda_strlen(str1) + 1;
    int n = cuda_strlen(str2) + 1;

    // Allocate memory for the distance matrix
    int* distance = (int*)malloc(m * n * sizeof(int));

    // Populate the distance matrix using the Levenshtein distance algorithm
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == 0) {
                distance[i * n + j] = j;
            } else if (j == 0) {
                distance[i * n + j] = i;
            } else if (str1[i - 1] == str2[j - 1]) {
                distance[i * n + j] = distance[(i - 1) * n + (j - 1)];
            } else {
                distance[i * n + j] = 1 + cuda_fmin(cuda_fmin(distance[(i - 1) * n + j], distance[i * n + (j - 1)]), distance[(i - 1) * n + (j - 1)]);
            }
        }
    }

    // Store the result and free the allocated memory
    int result = distance[m * n - 1];
    free(distance);

    return result;
}

/**
 * @brief CUDA kernel for computing the edit distance between two strings.
 *
 * This CUDA kernel divides the input strings among threads and computes the
 * Levenshtein distance locally for each thread. The results are stored in the
 * output array ED.
 *
 * @param str1 Pointer to the first input string.
 * @param str2 Pointer to the second input string.
 * @param ED   Pointer to the output array to store the computed edit distances.
 */
__global__ void EditDistanceKernel(char* str1, char* str2, int* ED) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int n_threads = blockDim.x*gridDim.x;

    // Partition the input strings for the current thread
    char* local_str1 = partitioning(str1, tid, n_threads);
    char* local_str2 = partitioning(str2, tid, n_threads);

    // Compute the Levenshtein distance locally for the current thread
    int local_ED = levenshteinDistance(local_str1, local_str2);

    // Store the local edit distance result in the output array
    ED[tid] = local_ED;
}

/**
 * @brief Perform Levenshtein distance calculation in a parallelized manner using OpenMP.
 *
 * This function partitions the input strings into multiple sections based on the number
 * of threads and calculates the Levenshtein distance for each section concurrently using
 * OpenMP parallelism. The results are combined to obtain the final distance.
 *
 * @param str1       The first input string.
 * @param str2       The second input string.
 * @param n_threads  Number of OpenMP threads.
 *
 * @return           The Levenshtein distance calculated in a parallelized manner.
 */
int threadPartitioning(const char* str1, const char* str2, int n_threads) {

    int* results = (int*)malloc(n_threads * sizeof(int));

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        char* local_str1 = partitioning(str1, tid, n_threads);
        char* local_str2 = partitioning(str2, tid, n_threads);

        results[tid] = levenshteinDistance(local_str1, local_str2);

        free(local_str1);
        free(local_str2);
    }

    int local_result = 0;
    for(int i = 0; i < n_threads; i++)
        local_result += results[i];

    return local_result;
}

/** LEGGIMI LEGGIMI LEGGIMI LEGGIMI LEGGIMI LEGGIMI
 * @brief Appends timing information to a CSV file.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * the timing for string generation, the timing for Kernel execution, 
 * the timing for overall program execution, the number of OpenMP threads used as input parameters.
 * (The number of CUDA threads is fixed)
 * It then appends the information to a CSV file in a formatted string.
 *
 * @param n_characters       Number of characters used in the program.
 * @param opt                Optimization level used in the program.
 * @param create_time        Time taken for string generation.
 * @param edit_distance_time Time taken for Edit Distance calculation.
 * @param execution_time     Total execution time.
 * @param omp_threads        Number of OpenMP threads used.
 * @param mpi_process        Number of MPI processes used.
 */
void printTimeToCSV(int n_characters, int opt, double create_time, double kernel_execution_time, double execution_time, int omp_threads) {
    // Define the file path
    char path[200];
    sprintf(path, "Informations/OMP_CUDA/opt%d/%d.csv", opt, n_characters); 
    char *filename = path;

    // Open the file for appending
    FILE *fp = fopen(filename, "a+");

    // Check if the file can be opened
    if (fp == NULL) {
        perror("ERROR during the file opening.");
        fprintf(stderr, "File %s can not be opened.\n", filename);
        return;
    }

    #ifdef L1_CACHE
        fprintf(fp, "OMP+CUDA_L1;%d;1024;%06f;%06f;%06f;\n", omp_threads, create_time, kernel_execution_time, execution_time);
    #else
        fprintf(fp, "OMP+CUDA;%d;1024;%06f;%06f;%06f;\n", omp_threads, create_time, kernel_execution_time, execution_time);
    #endif

    // Close the file
    fclose(fp);
}

/**
 * @brief Appends the Edit Distance OMP_CUDA result to a CSV file in a formatted string.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * the result of the Edit Distance, the number of OpenMP threads used as input parameters.
 * (The number of CUDA threads is fixed) 
 * It then appends the information to a CSV file in a formatted string.
 *
 * @param n_characters The number of characters used in the program.
 * @param opt          The type of optimization used.
 * @param distance     The result of the Edit Distance calculation.
 * @param omp_threads  The number of OpenMP threads used.
 */
void printResultToCSV(int n_characters, int opt, int distance, int omp_threads) {
    // Define the file path
    char path[200];
    sprintf(path, "EditDistanceReport/OMP_CUDA/opt%d/%d.csv", opt, n_characters); 
    char *filename = path;

    // Open the file for appending
    FILE *fp = fopen(filename, "a+");
    
    // Check if the file can be opened
    if (fp == NULL) {
        perror("ERROR during the file opening.");
        fprintf(stderr, "File %s cannot be opened.\n", filename);
        return;
    }

    // Change the print if is an execution with the L1 cache.
    #ifdef L1_CACHE
        fprintf(fp, "OMP+CUDA_L1;%d;1024;%d\n", omp_threads, distance);
    #else
        fprintf(fp, "OMP+CUDA;%d;1024;%d\n", omp_threads, distance);
    #endif

    // Close the file
    fclose(fp);
}

/**
 * @brief Perform Edit Distance computation using both OpenMP and CUDA.
 *
 * This function divides the workload between OpenMP and CUDA, computes the Edit Distance
 * between two input strings, and returns the elapsed time for the Kernel computation.
 *
 * @param str1       Pointer to the first input string.
 * @param str2       Pointer to the second input string.
 * @param n_threads  Number of OpenMP threads to use.
 * @param opt        Optimization level used in the program.
 * @return           Elapsed time for the Kernel computation.
 */
float editDistanceOnDevice(char* str1, char* str2, int n_threads, int opt) {
    // Divide the input strings for both GPU and host processing
    char* temp_gpu_str1 = partitioning(str1, 0, 2);
    int size_gpu_str1 = strlen(temp_gpu_str1);
    char* host_str1 = partitioning(str1, 1, 2);

    char* temp_gpu_str2 = partitioning(str2, 0, 2);
    int size_gpu_str2 = strlen(temp_gpu_str2);
    char* host_str2 = partitioning(str2, 1, 2);

    // Allocate and copy GPU memory
    char* gpu_str1;
    char* gpu_str2;
    CUDA_CHECK(cudaMalloc((void**)&gpu_str1, size_gpu_str1*sizeof(char)));
    CUDA_CHECK(cudaMalloc((void**)&gpu_str2, size_gpu_str2*sizeof(char)));
    CUDA_CHECK(cudaMemcpy(gpu_str1, temp_gpu_str1, size_gpu_str1*sizeof(char),cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(gpu_str2, temp_gpu_str2, size_gpu_str2*sizeof(char),cudaMemcpyHostToDevice));

   // Calculate the number of GPU threads blocks
    int n_grid = size_gpu_str1/20; //Just to make sure that each thread takes a few extra characters
    dim3 blockSize = 1024;
    dim3 gridSize = ((n_grid)/blockSize.x + 1);
    int cuda_threads = gridSize.x * blockSize.x;

    // Allocate GPU memory for Edit Distance results
    int* gpu_ED;
    CUDA_CHECK(cudaMalloc((void**)&gpu_ED, cuda_threads * sizeof(int)));

    // Create CUDA events for timing
    cudaEvent_t start, stop;
    CUDA_CHECK(cudaEventCreate(&start));
    CUDA_CHECK(cudaEventCreate(&stop));

    // Set cache configuration if optimization is enabled
    #ifdef L1_CACHE
        cudaFuncSetCacheConfig(EditDistanceKernel,cudaFuncCachePreferL1); //Function to prioritize the L1 cache to 48KB
    #endif

    // Record the start time and execute the CUDA kernel
    CUDA_CHECK(cudaEventRecord(start,0));
    EditDistanceKernel<<<gridSize, blockSize>>>(gpu_str1, gpu_str2, gpu_ED);
    CUDA_CHECK(cudaEventRecord(stop,0));

    // Perform thread partitioning on the host
    int final_ED = threadPartitioning(host_str1, host_str2, n_threads);

    // Synchronize GPU
    CUDA_CHECK(cudaDeviceSynchronize());

    // Copy results back to host, and calculate final Edit Distance
    int* result_gpu_ED = (int*)malloc(cuda_threads * sizeof(int));
    CUDA_CHECK(cudaMemcpy(result_gpu_ED,gpu_ED,cuda_threads * sizeof(int),cudaMemcpyDeviceToHost));

    for(int i = 0; i < cuda_threads; i++) {
        final_ED += result_gpu_ED[i];
    }

    // Calculate elapsed time
    float elapsed;
    CUDA_CHECK(cudaEventElapsedTime(&elapsed,start,stop));
    elapsed = elapsed/1000.f;

    // Cleanup GPU resources
    CUDA_CHECK(cudaEventDestroy(start));
    CUDA_CHECK(cudaEventDestroy(stop));
    CUDA_CHECK(cudaFree(gpu_str1));
    CUDA_CHECK(cudaFree(gpu_str2));
    CUDA_CHECK(cudaFree(gpu_ED));

    // Print result and free memory
    int n_characters = strlen(str1);
    printResultToCSV(n_characters, opt, final_ED, n_threads);

    free(temp_gpu_str1);
    free(temp_gpu_str2);
    free(host_str1);
    free(host_str2);
    free(result_gpu_ED);

    return elapsed;
}

/**
 * @brief Main function for launching the Edit Distance computation program.
 *
 * This function generates random strings, performs Edit Distance computation
 * using both OpenMP and CUDA, and prints the execution time to a CSV file.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 *             Usage: <program_name> <n_characters> <seed1> <seed2> <opt> <n_threads>
 * @return     0 if the program executes successfully, 1 otherwise.
 */
int main(int argc, char* argv[]) {

    // Check if the correct number of command line arguments is provided
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <n_characters> <seed1> <seed2> <opt> <n_threads>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    int n_characters = atoi(argv[1]);
    int seed1 = atoi(argv[2]);
    int seed2 = atoi(argv[3]);
    int opt = atoi(argv[4]);
    int n_threads = atoi(argv[5]);
    
    // Set the number of OpenMP threads
    omp_set_num_threads(n_threads);

    // Variables for tracking execution time
    double program_execution, create_time;
    float kernel_execution_time;
    
    // Record the start time for the entire program
    clock_t start_execution = clock();
    // Record the start time for string creation
    clock_t start_creation = clock();

    // Generate random strings
    char* str1 = generateRandomString(n_characters, seed1);
    char* str2 = generateRandomString(n_characters, seed2);

    // Record the end time for string creation
    clock_t end_creation = clock();

    // Calculate and store the time taken for string creation
    create_time = (double)(end_creation - start_creation) / CLOCKS_PER_SEC;

    // Perform Edit Distance computation and record the kernel execution time
    kernel_execution_time = editDistanceOnDevice(str1, str2, n_threads, opt);

    // Free memory allocated for strings
    free(str1);
    free(str2);

    // Record the end time for the entire program
    clock_t end_execution = clock();
    // Calculate and store the total program execution time
    program_execution = (double)(end_execution - start_execution) / CLOCKS_PER_SEC;

    // Print the execution times to a CSV file
    printTimeToCSV(n_characters, opt, create_time, kernel_execution_time, program_execution, n_threads);

    return 0;
}