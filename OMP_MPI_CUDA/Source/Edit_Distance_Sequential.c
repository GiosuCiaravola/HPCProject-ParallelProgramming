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
 * @brief Appends the Edit Distance result to a CSV file in a formatted string.
 *
 * This function takes the number of characters used in the program, the optimization used,
 * and the result of the Edit Distance as input parameters. It then appends the information
 * to a CSV file in a formatted string in the appropriated directory.
 *
 * @param n_characters The number of characters used in the program.
 * @param opt          The type of optimization used.
 * @param distance     The result of the Edit Distance calculation.
 * @param directory    The directory where to put the results.
 */
void printResultToCSV(int n_characters, int opt, int distance, char* directory) {
    // Define the file path
    char path[200];
    sprintf(path, "EditDistanceReport/%s/opt%d/%d.csv", directory, opt, n_characters);
    char *filename = path;

    // Open the file for appending
    FILE *fp = fopen(filename, "a+");
    
    // Check if the file can be opened
    if (fp == NULL) {
        perror("ERROR during the file opening.");
        fprintf(stderr, "File %s can not be opened.\n", filename);
        return;
    }

    // Print the formatted string to the CSV file
    fprintf(fp, "Sequential;0;0;%d\n", distance);

    // Close the file
    fclose(fp);
}

/**
 * @brief Entry point of the program to calculate and print Edit Distance result to CSV.
 *
 * This main function takes command line arguments, generates random strings, calculates
 * the Levenshtein distance, and prints the result to a CSV file.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line argument strings.
 *
 * @return     Returns 0 upon successful execution, 1 insted.
 */
int main(int argc, char* argv[]) {
    // Check if the correct number of command line arguments is provided
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <n_characters> <seed1> <seed2> <opt> <directory>\n", argv[0]);
        return 1;
    }

    // Parse command line arguments
    int n_characters = atoi(argv[1]);
    int seed1 = atoi(argv[2]);
    int seed2 = atoi(argv[3]);
    int opt = atoi(argv[4]);
    char* directory = argv[5];

    // Generate random strings
    char* X = generateRandomString(n_characters, seed1);
    char* Y = generateRandomString(n_characters, seed2);

    // Calculate Levenshtein distance
    int distance = levenshteinDistance(X, Y);

    // Free allocated memory
    free(X);
    free(Y);

    // Print result to CSV file
    printResultToCSV(n_characters, opt, distance, directory);

    return 0;
}