#ifndef BITOPERATION_H
#define BITOPERATION_H

#include <string.h>

// --- bases lower than 11 (represented as base-10 integers) ---

// Convert number from base 10 to a different base (returns as a base-10 represented integer)
// The existing function 'convertBase' already handles this (Base 10 to Base K).
int convertBase(int number, int base_new); 

// Convert fractional part from Base 10 to Base K (returns as a base-10 represented integer/double)
double convertBaseFracture(double num, int base);

// Addition in base K
int additionAnyBase( int number1, int number2, unsigned int base, unsigned int carry);

// Subtraction in base K
int subtractionAnyBase(int A, int B, unsigned int base);

// Multiplication in base K
int multiplicationAnyBase(int A, int B, unsigned int base);

// Division in base K (Returns only the quotient)
int divisionAnyBase(int dividend, int divisor, unsigned int base);

//get one's complement

// --- General Base Conversions ---

// Convert number from base K to base K' (Base K to Base K_tag)
int convertBaseToOtherBase(int number, unsigned int base_old, unsigned int base_new);

// --- Bitwise Operations (Using C's built-in support) ---

// Conversion from octal/hex string to binary string
char* convertToBase2(const char* number_str, int base_old);

// Common Bitwise Operations (Examples)
int bitwiseAND(int a, int b);
int bitwiseOR(int a, int b);
int bitwiseXOR(int a, int b);
int bitwiseNOT(int a);

#endif