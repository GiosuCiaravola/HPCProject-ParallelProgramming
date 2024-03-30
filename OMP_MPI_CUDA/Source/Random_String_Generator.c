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