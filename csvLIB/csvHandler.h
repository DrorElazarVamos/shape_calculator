#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vectorOps.h" // Includes vector, vectorSet definitions

#define MAX_LINE_LENGTH 1024

// --- CSV File Structure ---
typedef struct {
    FILE *file_ptr;
    char line_buffer[MAX_LINE_LENGTH];
    int current_line_number;
} CsvFile;

// --- Core CSV Function Prototypes ---

/**
 * @brief Opens a CSV file for reading
 * @param filename Path to the CSV file
 * @return Pointer to CsvFile structure, or NULL on failure
 */
CsvFile* csv_open(const char *filename);

/**
 * @brief Reads the next line from the CSV file
 * @param csv Pointer to CsvFile structure
 * @return true if successful, false on EOF or error
 */
bool csv_read_line(CsvFile *csv);

/**
 * @brief Retrieves the next field from the current line
 * @param csv Pointer to CsvFile structure
 * @return Pointer to field string, or NULL if no more fields
 */
char* csv_get_field(CsvFile *csv);

/**
 * @brief Closes the CSV file and frees memory
 * @param csv Pointer to CsvFile structure
 */
void csv_close(CsvFile *csv);

/**
 * @brief Reads a set of vectors from CSV file.
 * Assumes the first 3 columns are X, Y, Z and the 4th is Magnitude (ignored).
 * @param filename Path to the CSV file
 * @return vectorSet structure containing the vectors
 */
vectorSet csv_read_vector_set(const char *filename);

#endif // CSV_HANDLER_H