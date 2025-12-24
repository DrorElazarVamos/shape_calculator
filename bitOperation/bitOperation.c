#include "bitOperation.h" // Includes function prototypes
#include <stdio.h>    // For printf, fprintf, NULL
#include <stdlib.h>   // For malloc, free, strtol
#include <string.h>   // For strlen, strcat, memmove
#include <math.h>     // For floor

// --- Helper Functions ---

/**
 * @brief Converts a number from an arbitrary Base K (represented as a base-10 integer) to Base 10.
 * @param number The number in Base K representation (e.g., 123 for 123_k).
 * @param base_old The original base K (2 <= K < 11).
 * @return int The equivalent value in Base 10.
 */
int convertBaseTo10(int number, unsigned int base_old) {
    int result = 0;
    int power = 1; // Represents base_old^0, base_old^1, etc.

    while (number > 0) {
        int digit = number % 10; // Extract the digit in base K
        result += digit * power;
        power *= base_old;
        number /= 10;
    }
    return result;
}


// --- Arithmetic Helper ---

/**
 * @brief Helper function for multiplication: Multiplies an entire number (A) by a single digit (B) in base K.
 * @param A The first number.
 * @param B The single digit from the second number.
 * @param base The target base K.
 * @param carry The carry-in from the previous step.
 * @return int The product A * B (represented as a base-10 integer).
 */
int multiplicationSingleDigit(int A, int B, unsigned int base, unsigned int carry){
    if ( A == 0) { // Should be A == 0, not B == 0
        return carry;
    }

    unsigned int d1 = A % 10;

    unsigned int product_sum = (d1 * B) + carry;

    unsigned int result_digits = product_sum % base;
    unsigned int next_carry = product_sum / base;
    
    return result_digits + 10 * multiplicationSingleDigit(A / 10, B, base, next_carry);
}


// --- Base Conversion Functions (K < 11) ---

/**
 * @brief Converts a number from Base 10 to Base K (represented as a base-10 integer).
 * @param number The Base 10 number.
 * @param base The target base K (2 <= K < 11).
 * @return int The number's representation in Base K.
 */
int convertBase(int number, int base){
    if (number == 0){
        return 0;
    }
    int digit = number % base;

    return digit + 10 * convertBase(number / base, base);
}

/**
 * @brief Converts Base 10 fractional part to Base K (approximated representation).
 * @param num The Base 10 fractional part (0 < num < 1).
 * @param base The target base K (2 <= K < 11).
 * @return double The number's fractional part representation in Base K (e.g., 0.101 for 0.625_10 in base 2).
 */
double convertBaseFracture(double num, int base){
    double result = 0.0;
    double power_of_ten = 0.1;
    int max_precision = 5; // Limit to 5 digits for simplicity

    for (int i = 0; i < max_precision; ++i) {
        if (num <= 0.0) break;
        
        num *= base;
        int digit = (int)floor(num);
        num -= digit;
        
        // Append to result using base-10 representation for storage
        result += digit * power_of_ten;
        power_of_ten /= 10.0;
    }
    return result;
}

/**
 * @brief Converts number from Base K to Base K' (Base K_old to Base K_new).
 * @param number The number in Base K_old representation.
 * @param base_old The original base K.
 * @param base_new The new target base K'.
 * @return int The number's representation in Base K'.
 */
int convertBaseToOtherBase(int number, unsigned int base_old, unsigned int base_new){
    // 1. Convert from Base K_old to Base 10
    int base10_num = convertBaseTo10(number, base_old);
    
    // 2. Convert from Base 10 to Base K_new
    return convertBase(base10_num, base_new);
}


// --- Arithmetic Functions (Base K < 11) ---

/**
 * @brief Recursively calculates the addition of two numbers in an arbitrary base (K < 11).
 * @param number1 The first number.
 * @param number2 The second number.
 * @param base The target base K.
 * @param carry The carry-in from the previous step.
 * @return int The sum (represented as a base-10 integer).
 */
int additionAnyBase( int number1, int number2, unsigned int base, unsigned int carry){
    if (number1 == 0 && number2 == 0) {
        return carry; 
    }
    unsigned int d1 = number1 % 10; 
    unsigned int d2 = number2 % 10;

    unsigned int sum = d1 + d2 + carry;
    unsigned int result_digit = sum % base;
    unsigned int next_carry = sum / base;

    return result_digit + 10 * additionAnyBase(number1 / 10, number2 / 10, base, next_carry);
}

/**
 * @brief Recursively calculates the subtraction of two numbers in an arbitrary base (K < 11).
 * @param A The larger number (minuend).
 * @param B The smaller number (subtrahend).
 * @param base The target base K.
 * @return int The difference (A - B) (represented as a base-10 integer).
 */
int subtractionAnyBase(int A, int B, unsigned int base){
    if (A == 0 && B == 0) {
        return 0; 
    }

    int d1 = A % 10;
    int d2 = B % 10;
    
    int result_digit;
    int next_borrow = 0;

    if (d1 >= d2) {
        result_digit = d1 - d2;
    } else {
        // Must borrow: d1 becomes d1 + base
        result_digit = (d1 + base) - d2;
        next_borrow = 1; 
    }

    // The borrow is applied by subtracting 1 from the next digit of A
    return result_digit + 10 * subtractionAnyBase(A / 10 - next_borrow, B / 10, base);
}

/**
 * @brief Recursively calculates the multiplication of two numbers in an arbitrary base (K < 11).
 * @param A The first number.
 * @param B The second number.
 * @param base The target base K.
 * @return int The product (represented as a base-10 integer).
 */
int multiplicationAnyBase(int A, int B, unsigned int base){
    if (A == 0 || B == 0){
        return 0;
    }

    unsigned int d2 = B % 10;

    int remain = multiplicationAnyBase(A, B / 10, base);
    int current_product = multiplicationSingleDigit(A, d2, base, 0);
    int shifted_remain = 10 * remain; // Shift by 10 for base-10 representation

    // Use the base-K addition function
    return additionAnyBase(current_product, shifted_remain, base, 0);
}

/**
 * @brief Calculates the division (quotient only) of two numbers in an arbitrary base (K < 11) using Base 10 as an intermediary.
 * @param dividend The dividend.
 * @param divisor The divisor.
 * @param base The target base K.
 * @return int The quotient (represented as a base-10 integer), or -1 on error.
 */
int divisionAnyBase(int dividend, int divisor, unsigned int base){
    if (divisor == 0) {
        fprintf(stderr, "Error: Division by zero.\n");
        return -1; 
    }

    // 1. Convert to Base 10
    int dividend_10 = convertBaseTo10(dividend, base);
    int divisor_10 = convertBaseTo10(divisor, base);

    // 2. Perform Base 10 division
    int quotient_10 = dividend_10 / divisor_10;

    // 3. Convert quotient back to Base K
    return convertBase(quotient_10, base);
}


// --- Binary Conversion (Hex/Octal to Binary String) ---

// Lookup tables
const char* octal_map[] = {"000", "001", "010", "011", "100", "101", "110", "111"};
const char* hex_map[] = {"0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111", 
                         "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};

/**
 * @brief Converts a number represented as an Octal or Hexadecimal string to a Binary string.
 * @param number_str The input number string (e.g., "1A" for hex, "72" for octal).
 * @param base_old The original base (8 or 16).
 * @return char* A dynamically allocated string containing the binary representation, or NULL on error.
 */
char* convertToBase2(const char* number_str, int base_old){
    if (base_old != 8 && base_old != 16) {
        fprintf(stderr, "Error: Base must be 8 or 16.\n");
        return NULL;
    }

    int len = strlen(number_str);
    int group_size = (base_old == 8) ? 3 : 4;
    
    // Allocate max possible size: len * group_size + 1 for '\0'
    char* binary_str = (char*)malloc(len * group_size + 1);
    if (!binary_str) return NULL;
    
    binary_str[0] = '\0'; 

    for (int i = 0; i < len; ++i) {
        char digit_char = number_str[i];
        int val;

        if (digit_char >= '0' && digit_char <= '9') {
            val = digit_char - '0';
        } else if (base_old == 16 && (digit_char >= 'A' && digit_char <= 'F')) {
            val = digit_char - 'A' + 10;
        } else if (base_old == 16 && (digit_char >= 'a' && digit_char <= 'f')) {
            val = digit_char - 'a' + 10;
        } else {
            fprintf(stderr, "Error: Invalid digit in input string.\n");
            free(binary_str);
            return NULL;
        }
        
        // Append the corresponding binary group
        strcat(binary_str, (base_old == 8) ? octal_map[val] : hex_map[val]);
    }
    
    // Remove leading zeros (e.g., "00101" -> "101")
    char* p = binary_str;
    // Skip leading '0's unless it's the only character
    while (p[0] == '0' && p[1] != '\0') {
        p++;
    }
    if (p != binary_str) {
        memmove(binary_str, p, strlen(p) + 1);
    }

    return binary_str;
}


// --- Common Binary Operations (Bitwise) ---

/**
 * @brief Performs a bitwise AND operation.
 */
int bitwiseAND(int a, int b) {
    return a & b;
}

/**
 * @brief Performs a bitwise OR operation.
 */
int bitwiseOR(int a, int b) {
    return a | b;
}

/**
 * @brief Performs a bitwise XOR operation.
 */
int bitwiseXOR(int a, int b) {
    return a ^ b;
}

/**
 * @brief Performs a bitwise NOT (one's complement) operation.
 */
int bitwiseNOT(int a) {
    return ~a;
}