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
 * @brief Appends timing information to a CSV file in a formatted string.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * the timing for string generation, the timing for Edit Distance algorithm execution, 
 * the timing for overall program execution, the number of OpenMP threads used, 
 * and the number of MPI processes used as input parameters (just for the compare with the parallel version).
 * It then appends the information to a CSV file in a formatted string.
 *
 * @param n_characters      The number of characters used in the program.
 * @param opt               The type of optimization used.
 * @param create_time       The time taken for string generation.
 * @param edit_distance_time The time taken for the Edit Distance algorithm execution.
 * @param execution_time    The total time taken for the entire program execution.
 * @param omp_threads       The number of OpenMP threads (just for the split).
 * @param mpi_process       The number of MPI processes (just for the split).
 * @param directory    The directory where to put the results.
 */
void printTimeToCSV(int n_characters, int opt, double create_time, double edit_distance_time, double execution_time, int omp_threads, int mpi_process) {
    // Define the file path
    char path[200];
    sprintf(path, "Informations/OMP_MPI/opt%d/%d.csv", opt, n_characters); 
    char *filename = path;

    // Open the file for appending
    FILE *fp = fopen(filename, "a+");
    
    // Check if the file can be opened
    if (fp == NULL) {
        perror("ERROR during the file opening.");
        fprintf(stderr, "File %s cannot be opened.\n", filename);
        return;
    }

    fprintf(fp, "Approximate;%d;%d;%06f;0.0;%06f;%06f;\n", omp_threads, mpi_process, create_time, edit_distance_time, execution_time);

    // Close the file
    fclose(fp);
}

/**
 * @brief Appends the Edit Distance Approximate result to a CSV file in a formatted string.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * the result of the Edit Distance, the number of OpenMP threads used, and the number of MPI processes used
 * as input parameters (just for the compare with the parallel version). 
 * It then appends the information to a CSV file in a formatted string.
 *
 * @param n_characters The number of characters used in the program.
 * @param opt          The type of optimization used.
 * @param distance     The result of the Edit Distance calculation.
 * @param omp_threads  The number of OpenMP threads (just for the split).
 * @param mpi_process  The number of MPI processes (just for the split).
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
    fprintf(fp, "Approximate;%d;%d;%d\n", omp_threads, mpi_process, distance);

    // Close the file
    fclose(fp);
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
 * @brief Main function for the Approximate Edit Distance calculation program.
 *
 * This program generates random strings, partitions them,
 * calculates the Edit Distance, and records the timing information. 
 * The results are then printed to CSV files.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 *
 * @return Returns 0 on successful execution, 1 on incorrect number of arguments.
 */
int main(int argc, char* argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 7) {
        fprintf(stderr, "Usage: %s <n_characters> <seed1> <seed2> <opt> <omp_threads> <mpi_process>\n", argv[0]);
        return 1; 
    }

    // Parse command line arguments
    int n_characters = atoi(argv[1]);
    int seed1 = atoi(argv[2]);
    int seed2 = atoi(argv[3]);
    int opt = atoi(argv[4]);
    int omp_threads = atoi(argv[5]);
    int mpi_process = atoi(argv[6]);

    // Variables for timing
    struct timeval execution_start, execution_stop, string_generation_start, string_generation_stop, edit_distance_start, edit_distance_stop;
    double execution_time, create_time, edit_distance_time;

    // Record the start time of the entire program
    gettimeofday(&execution_start, NULL);

    // Generate random strings
    gettimeofday(&string_generation_start, NULL);
    char* X = generateRandomString(n_characters, seed1);
    char* Y = generateRandomString(n_characters, seed2);
    gettimeofday(&string_generation_stop, NULL);

    // Calculate the time taken for string generation
    create_time = getTotalTime(string_generation_start, string_generation_stop);

    int ED = 0;

    // Record the start time of the Edit Distance calculation
    gettimeofday(&edit_distance_start, NULL);

    // Partition strings and calculate Edit Distance
    for (int i = 0; i < mpi_process; i++) {
        char* process_str1 = partitioning(X, i, mpi_process);
        char* process_str2 = partitioning(Y, i, mpi_process);

        for (int j = 0; j < omp_threads; j++) {
            char* thread_str1 = partitioning(process_str1, j, omp_threads);
            char* thread_str2 = partitioning(process_str2, j, omp_threads);

            ED += levenshteinDistance(thread_str1, thread_str2);

            free(thread_str1);
            free(thread_str2);
        }

        free(process_str1);
        free(process_str2);
    }

    // Record the stop time of the Edit Distance calculation
    gettimeofday(&edit_distance_stop, NULL);

    // Calculate the time taken for the Edit Distance calculation
    edit_distance_time = getTotalTime(edit_distance_start, edit_distance_stop);

    // Record the stop time of the entire program
    gettimeofday(&execution_stop, NULL);

    // Calculate the total execution time
    execution_time = getTotalTime(execution_start, execution_stop);

    // Print timing information to CSV
    printTimeToCSV(n_characters, opt, create_time, edit_distance_time, execution_time, omp_threads, mpi_process);

    // Print Edit Distance result to CSV
    printResultToCSV(n_characters, opt, ED, omp_threads, mpi_process);

    // Free allocated memory
    free(X);
    free(Y);

    return 0;
}