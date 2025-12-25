#include "universal.h"
#include <stdio.h>  // FIXED: Required for fprintf, stderr
#include <stdlib.h> // FIXED: Required for exit, EXIT_FAILURE

// FIXED: Return type is 'void' because we don't need to return a value back to the macro
void check_memory_allocation(void *ptr, const char *file, int line) {
    if (ptr == NULL) {
        // FIXED: fprintf requires 'stderr' as the first argument
        fprintf(stderr, "Failed to allocate memory in %s line %d\n", file, line);
        
        // We must exit, otherwise the program will crash later when using the NULL pointer
        exit(EXIT_FAILURE); 
    }
}