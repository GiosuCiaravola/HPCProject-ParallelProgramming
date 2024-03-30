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
#include <string.h>
#include <omp.h>
#include <mpi.h>
#include <math.h>
#include <sys/time.h>
#include "Random_String_Generator.h"
//Constant for the time conversion
#define SEC_CONV 1000000

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
int levenshteinDistance(const char* str1, const char* str2) {
    // Lengths of input strings plus one for the null terminator
    int m = strlen(str1) + 1;
    int n = strlen(str2) + 1;

    // Allocate memory for the distance matrix
    int* distance = (int*)malloc(m * n * sizeof(int));

    // Check for memory allocation failure
    if (distance == NULL) {
        fprintf(stderr, "ERROR in memory allocation.\n");
        return -1;
    }

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
                distance[i * n + j] = 1 + fmin(fmin(distance[(i - 1) * n + j], distance[i * n + (j - 1)]),
                                                distance[(i - 1) * n + (j - 1)]);
            }
        }
    }

    // Store the result and free the allocated memory
    int result = distance[m * n - 1];
    free(distance);

    return result;
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
char* partitioning(const char* str, int id, int n_id) {
    int len = strlen(str);

    // Calculate the lengths and starting points of local sections of work
    int local_len = len / n_id; 
    int remaining = len % n_id; 
    int local_start = id * local_len + fmin(id, remaining); 
    local_len += (id < remaining) ? 1 : 0; 

    // Allocate memory for the local substring
    char* local_str = (char*)malloc((local_len + 1) * sizeof(char));
    
    // Copy the partitioned substring into the allocated memory
    strncpy(local_str, str + local_start, local_len);
    local_str[local_len] = '\0';

    return local_str;
}

/**
 * @brief Calculates the duration between two time points and converts it to seconds.
 *
 * This function takes two `struct timeval` time points, calculates the duration between them,
 * and converts the result to seconds for easier interpretation. It uses a conversion factor
 * `SEC_CONV` to handle the microsecond part of the time.
 *
 * @param start The starting time point.
 * @param stop  The ending time point.
 *
 * @return      The duration between the two time points in seconds.
 */
double getTotalTime(struct timeval start, struct timeval stop) {
    unsigned long long start_time, stop_time;

    // Convert timeval to microseconds
    start_time = (unsigned long long)start.tv_sec * SEC_CONV + start.tv_usec;
    stop_time = (unsigned long long)stop.tv_sec * SEC_CONV + stop.tv_usec;

    // Calculate the duration in microseconds and convert it to seconds
    return (double)(stop_time - start_time) / (double)SEC_CONV;
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
    // Array to store individual thread results
    int results[n_threads];

    // OpenMP parallel region
    #pragma omp parallel
    {
        // Get the thread ID
        int tid = omp_get_thread_num();

        // Partition the input strings for each thread
        char* local_str1 = partitioning(str1, tid, n_threads);
        char* local_str2 = partitioning(str2, tid, n_threads);

        // Calculate Levenshtein distance for each thread
        results[tid] = levenshteinDistance(local_str1, local_str2);

        // Free allocated memory for local strings
        free(local_str1);
        free(local_str2);
    }

    // Combine individual thread results
    int local_result = 0;
    for (int i = 0; i < n_threads; i++)
        local_result += results[i];

    return local_result;
}

/**
 * @brief Appends timing information to a CSV file.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * the timing for string generation, the timing for rank communication, 
 * the timing for Edit Distance algorithm execution, 
 * the timing for overall program execution, the number of OpenMP threads used, 
 * and the number of MPI processes used as input parameters.
 * It then appends the information to a CSV file in a formatted string.
 *
 * @param n_characters       Number of characters used in the program.
 * @param opt                Optimization level used in the program.
 * @param create_time        Time taken for string generation.
 * @param create_time        Time taken for rank communication.
 * @param edit_distance_time Time taken for Edit Distance calculation.
 * @param execution_time     Total execution time.
 * @param omp_threads        Number of OpenMP threads used.
 * @param mpi_process        Number of MPI processes used.
 */
void printTimeToCSV(int n_characters, int opt, double create_time, double comunication_time, double edit_distance_time, double execution_time, int omp_threads, int mpi_process) {
    // Define the file path
    char path[200];
    sprintf(path, "Informations/OMP_MPI/opt%d/%d.csv", opt, n_characters); 
    char *filename = path;

    // Open the file for appending
    FILE *fp = fopen(filename, "a+");

    // Check if the file can be opened
    if (fp == NULL) {
        perror("ERROR during the file opening.");
        fprintf(stderr, "File %s can not be opened.\n", filename);
        return;
    }

    // Print timing information to the CSV file
    fprintf(fp, "OMP+MPI;%d;%d;%06f;%06f;%06f;%06f;\n", omp_threads, mpi_process, create_time, comunication_time, edit_distance_time, execution_time);

    // Close the file
    fclose(fp);
}

/**
 * @brief Appends the Edit Distance OMP_MPI result to a CSV file in a formatted string.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * the result of the Edit Distance, the number of OpenMP threads used, and the number of 
 * MPI processes used as input parameters. 
 * It then appends the information to a CSV file in a formatted string.
 *
 * @param n_characters The number of characters used in the program.
 * @param opt          The type of optimization used.
 * @param distance     The result of the Edit Distance calculation.
 * @param omp_threads  The number of OpenMP threads used.
 * @param mpi_process  The number of MPI processes used.
 */
void printResultToCSV(int n_characters, int opt, int distance, int omp_threads, int mpi_process) {
    // Define the file path
    char path[200];
    sprintf(path, "EditDistanceReport/OMP_MPI/opt%d/%d.csv", opt, n_characters); 
    char *filename = path;

    // Open the file for appending
    FILE *fp = fopen(filename, "a+");
    
    // Check if the file can be opened
    if (fp == NULL) {
        perror("ERROR during the file opening.");
        fprintf(stderr, "File %s cannot be opened.\n", filename);
        return;
    }

    // Print the formatted string to the CSV file
    fprintf(fp, "OMP+MPI;%d;%d;%d\n", omp_threads, mpi_process, distance);

    // Close the file
    fclose(fp);
}

/**
 * @brief Main function for the parallelized Edit Distance calculation program using MPI and OpenMP.
 *
 * This program generates random strings, partitions them for parallel processing,
 * calculates the Edit Distance in a distributed manner, and records the timing information.
 * The results are then printed to CSV files. MPI is used for parallelism among processes,
 * and OpenMP is used for parallelism within each process.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 *
 * @return Returns 0 on successful execution, 1 on incorrect number of arguments.
 */
int main(int argc, char** argv) {
    // MPI Initialization
    MPI_Init(&argc, &argv);

    // Timing variables
    struct timeval execution_start, execution_stop, string_generation_start, string_generation_stop, edit_distance_start, edit_distance_stop;
    double execution_time, create_time, edit_distance_time, communication_time;

    // MPI rank and size variables
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check if the correct number of command line arguments is provided
    if (argc != 7) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <n_characters> <seed1> <seed2> <opt> <n_threads>\n", argv[0]);
        }
        MPI_Finalize();
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

    // Record the start time of the entire program
    gettimeofday(&execution_start, NULL);

    // Generate random strings
    gettimeofday(&string_generation_start, NULL);
    char* str1 = generateRandomString(n_characters, seed1);
    char* str2 = generateRandomString(n_characters, seed2);
    gettimeofday(&string_generation_stop, NULL);

    // Calculate the time taken for string generation
    create_time = getTotalTime(string_generation_start, string_generation_stop);

    // Record the start time of the Edit Distance calculation
    gettimeofday(&edit_distance_start, NULL);

    // Partition strings and calculate Edit Distance in parallel
    char* local_str1 = partitioning(str1, rank, size);
    char* local_str2 = partitioning(str2, rank, size);

    // Synchronize before calculating Edit Distance in parallel
    MPI_Barrier(MPI_COMM_WORLD);

    int local_result = threadPartitioning(local_str1, local_str2, n_threads);
    int ED;

    //Calculation the communication time
    double temptime;
    temptime = MPI_Wtime();

    // Sum the partial results
    MPI_Reduce(&local_result, &ED, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    communication_time = MPI_Wtime() - temptime;

    free(local_str1);
    free(local_str2);

    // Record the stop time of the Edit Distance calculation
    gettimeofday(&edit_distance_stop, NULL);

    // Calculate the time taken for the Edit Distance calculation
    edit_distance_time = getTotalTime(edit_distance_start, edit_distance_stop);

    // Record the stop time of the entire program
    gettimeofday(&execution_stop, NULL);

    // Calculate the total execution time
    execution_time = getTotalTime(execution_start, execution_stop);

    // Print timing information and Edit Distance result to CSV (only by rank 0)
    if (rank == 0) {
        printTimeToCSV(n_characters, opt, create_time, communication_time, edit_distance_time, execution_time, n_threads, size);
        printResultToCSV(n_characters, opt, ED, n_threads, size);
    }

    free(str1);
    free(str2);

    // MPI Finalization
    MPI_Finalize();

    return 0;
}