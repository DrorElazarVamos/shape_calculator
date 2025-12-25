#ifndef TESTER_FILE_H
#define TESTER_FILE_H

#include "vectorOps.h"
#include "csvHandler.h"

// --- Function Pointer Types ---
// Note: 'vector' is already a pointer type
typedef double (*VolumeOperation)(vector vectors[], double k);
typedef double (*BinaryVectorOperation)(vector v1, vector v2);
typedef vector (*CrossOperation)(vector v1, vector v2);

// --- Test Runner Function Prototypes ---

/**
 * @brief Runs volume calculation tests on CSV data
 * @param csv Opened CSV file pointer
 * @param operation Function pointer to volume calculation function
 * @param test_name Name of the test for display
 * @param k_value The k constant for volume calculation (1.0 for parallelepiped, 6.0 for pyramid)
 */
void run_volume_tests(CsvFile *csv, VolumeOperation operation, const char *test_name, double k_value);

/**
 * @brief Runs scalar product tests on CSV data
 * @param csv Opened CSV file pointer
 * @param operation Function pointer to scalar product function
 */
void run_scalar_product_tests(CsvFile *csv, BinaryVectorOperation operation);

/**
 * @brief Runs cross product tests on CSV data
 * @param csv Opened CSV file pointer
 * @param operation Function pointer to cross product function
 */
void run_cross_product_tests(CsvFile *csv, CrossOperation operation);

#endif // TESTER_FILE_H