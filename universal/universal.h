#ifndef UNIVERSAL_H
#define UNIVERSAL_H

#include <stdlib.h> // Needed for pointers/NULL

/**
 * @brief Macro to check if a pointer is NULL.
 * Usage: vector v = malloc(...); Cmalloc(v);
 * If v is NULL, it prints an error with the filename and line number, then exits.
 */
#define Cmalloc(ptr) check_memory_allocation(ptr, __FILE__, __LINE__)

/**
 * @brief Helper function for the macro.
 * Defined as 'void' because it either passes silently or kills the program.
 */
void check_memory_allocation(void *ptr, const char *file, int line);

#endif // UNIVERSAL_H