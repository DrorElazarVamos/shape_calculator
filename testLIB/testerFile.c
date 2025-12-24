#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include "vectorOps.h"
#include "csvHandler.h"
#include "testerFile.h"

// --- Define Test Case Struct ---
typedef struct {
    vector v1;
    vector v2;
    vector v3;
    double expected_volume;
} TestCase;

// --- Helper Prototypes ---
static bool map_4_fields_to_vector(CsvFile *file, vector *v_out);
static bool read_test_case_row(CsvFile *file, TestCase *test_case);
static bool vectors_are_coplanar(vector v1, vector v2, vector v3, double tolerance);

// Helper function to check if three vectors are coplanar
static bool vectors_are_coplanar(vector v1, vector v2, vector v3, double tolerance) {
    vector cross = crossProduct(v1, v2);
    double scalar_triple = scalaricProduct(cross, v3);
    return fabs(scalar_triple) < tolerance;
}

// Function to read all 13 fields of a single test case line
static bool read_test_case_row(CsvFile *file, TestCase *test_case) {
    char *field_str;

    // --- Read V1 (4 fields) ---
    if (!map_4_fields_to_vector(file, &test_case->v1)) return false;

    // --- Read V2 (4 fields) ---
    if (!map_4_fields_to_vector(file, &test_case->v2)) return false;

    // --- Read V3 (4 fields) ---
    if (!map_4_fields_to_vector(file, &test_case->v3)) return false;
    
    // --- Read Expected Volume (1 field) ---
    field_str = csv_get_field(file);
    if (field_str == NULL) return false;
    test_case->expected_volume = atof(field_str);

    return true;
}

// Helper function to map 4 sequential fields into one vector struct
static bool map_4_fields_to_vector(CsvFile *file, vector *v_out) {
    char *field_str;
    if (!file || !v_out) return false;

    // 1. Read X, Y, Z components
    for (int i = 0; i < 3; i++) {
        field_str = csv_get_field(file);
        if (field_str == NULL) return false;
        v_out->direction[i] = atof(field_str);
    }
    
    // 4. Read Magnitude
    field_str = csv_get_field(file);
    if (field_str == NULL) return false;
    v_out->magnitude = atof(field_str);

    return true;
}

// --- Test Runner Functions ---

void run_volume_tests(CsvFile *csv, VolumeOperation operation, const char *test_name, double k_value) {
    TestCase current_test;
    int test_count = 0;
    int passed_count = 0;
    int failed_count = 0;
    int error_count = 0;
    
    printf("\n=== Testing %s (k=%.1f) ===\n", test_name, k_value);
    if (k_value == 6.0) {
        printf("Note: CSV contains parallelepiped volumes. Expected = Parallelepiped / 6\n");
    }

    // Rewind file and skip header
    rewind(csv->file_ptr);
    csv->current_line_number = 0;
    if (!csv_read_line(csv)) {
        printf("ERROR: Cannot read CSV header\n");
        return;
    }

    while (csv_read_line(csv)) {
        test_count++;
        
        if (read_test_case_row(csv, &current_test)) {
            vector vectors[3] = {current_test.v1, current_test.v2, current_test.v3};
            double calculated_volume = operation(vectors, k_value);
            
            // Adjust expected volume based on k value
            double expected_volume = current_test.expected_volume / k_value;
            
            // Validation: if expected volume is ~0, vectors should be coplanar
            if (fabs(expected_volume) < 0.001) {
                if (!vectors_are_coplanar(current_test.v1, current_test.v2, current_test.v3, 0.001)) {
                    printf("Test %d: WARNING - Expected volume ~0 but vectors not coplanar\n", test_count);
                }
            }
            
            // Compare the result (0.1% tolerance)
            double tolerance = 0.001; 
            if (fabs(calculated_volume - expected_volume) < tolerance) {
                printf("Test %d: PASS (Volume: %.3lf)\n", test_count, calculated_volume);
                passed_count++;
            } else {
                printf("Test %d: FAIL! (Calculated: %.3lf, Expected: %.3lf, Diff: %.6lf)\n", 
                       test_count, calculated_volume, expected_volume, 
                       fabs(calculated_volume - expected_volume));
                failed_count++;
            }
        } else {
            printf("Test %d: ERROR - Could not parse all 13 fields from the row.\n", test_count);
            error_count++;
        }
    }
    
    printf("\n--- %s Summary ---\n", test_name);
    printf("Total Tests: %d | Passed: %d | Failed: %d | Errors: %d\n", 
           test_count, passed_count, failed_count, error_count);
    
    if (passed_count == test_count && test_count > 0) {
        printf("✓ All tests passed!\n");
    } else if (failed_count > 0) {
        printf("✗ Some tests failed. Review output above.\n");
    }
    printf("\n");
}

void run_scalar_product_tests(CsvFile *csv, BinaryVectorOperation operation) {
    TestCase current_test;
    int test_count = 0;
    int error_count = 0;
    
    printf("\n=== Testing Scalar Product ===\n");

    // Rewind file and skip header
    rewind(csv->file_ptr);
    csv->current_line_number = 0;
    if (!csv_read_line(csv)) {
        printf("ERROR: Cannot read CSV header\n");
        return;
    }

    while (csv_read_line(csv)) {
        test_count++;
        
        if (read_test_case_row(csv, &current_test)) {
            // Test V1 · V2
            double result_v1_v2 = operation(current_test.v1, current_test.v2);
            printf("Test %d: V1 · V2 = %.3lf\n", test_count, result_v1_v2);
            
            // Test V1 · V3
            double result_v1_v3 = operation(current_test.v1, current_test.v3);
            printf("        V1 · V3 = %.3lf\n", result_v1_v3);
            
            // Test V2 · V3
            double result_v2_v3 = operation(current_test.v2, current_test.v3);
            printf("        V2 · V3 = %.3lf\n", result_v2_v3);
        } else {
            printf("Test %d: ERROR - Could not parse test case\n", test_count);
            error_count++;
        }
    }
    
    printf("\n--- Scalar Product Summary ---\n");
    printf("Total test cases processed: %d | Errors: %d\n\n", test_count, error_count);
}

void run_cross_product_tests(CsvFile *csv, CrossOperation operation) {
    TestCase current_test;
    int test_count = 0;
    int error_count = 0;
    
    printf("\n=== Testing Cross Product ===\n");

    // Rewind file and skip header
    rewind(csv->file_ptr);
    csv->current_line_number = 0;
    if (!csv_read_line(csv)) {
        printf("ERROR: Cannot read CSV header\n");
        return;
    }

    while (csv_read_line(csv)) {
        test_count++;
        
        if (read_test_case_row(csv, &current_test)) {
            // Test V1 × V2
            vector result = operation(current_test.v1, current_test.v2);
            printf("Test %d: V1 × V2 = [%.3lf, %.3lf, %.3lf] (mag: %.3lf)\n", 
                   test_count, result.direction[0], result.direction[1], 
                   result.direction[2], result.magnitude);
            
            // Verify perpendicularity (dot product should be ~0)
            double dot_v1 = scalaricProduct(result, current_test.v1);
            double dot_v2 = scalaricProduct(result, current_test.v2);
            
            if (fabs(dot_v1) > 0.001 || fabs(dot_v2) > 0.001) {
                printf("        WARNING: Result not perpendicular (V1·result=%.3lf, V2·result=%.3lf)\n",
                       dot_v1, dot_v2);
            }
        } else {
            printf("Test %d: ERROR - Could not parse test case\n", test_count);
            error_count++;
        }
    }
    
    printf("\n--- Cross Product Summary ---\n");
    printf("Total test cases processed: %d | Errors: %d\n\n", test_count, error_count);
}