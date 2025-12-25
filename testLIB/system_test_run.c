#include <stdio.h>
#include "testerFile.h"
#include "csvHandler.h"
#include "vectorOps.h"

int main(int argc, char *argv[]) {
    printf("=== SYSTEM INTEGRATION TEST ===\n");
    
    // Default file if none provided
    const char *test_file = "full_test_suite.csv";
    
    // If user provided a file argument, use that instead
    if (argc > 1) {
        test_file = argv[1];
    }

    printf("Loading test file: %s\n", test_file);

    // 1. Open CSV
    CsvFile *csv = csv_open(test_file);
    if (!csv) {
        fprintf(stderr, "FATAL: Could not open '%s'.\n", test_file);
        fprintf(stderr, "Usage: ./system_test [path/to/csv_file.csv]\n");
        return 1;
    }

    // 2. Run Tests
    run_volume_tests(csv, volumeParallelepiped, "System Volume Check", 1.0);
    run_cross_product_tests(csv, crossProduct);
    run_scalar_product_tests(csv, scalaricProduct);

    csv_close(csv);
    printf("=== SYSTEM TEST COMPLETE ===\n");
    return 0;
}