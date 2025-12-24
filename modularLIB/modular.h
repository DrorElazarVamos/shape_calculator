#ifndef MODULAR_H
#define MODULAR_H

// --- Libraries ---
#include <math.h>
#include <stdbool.h>

// --- functions ---

/**
 * @brief Calculates the greatest common divisor (GCD) of two integers.
 * @param a The first integer.
 * @param b The second integer.
 * @return The GCD of a and b (always non-negative).
*/
int GCD(int a, int b);

/**
 * @brief Checks if two numbers are coprime (i.e., their GCD is 1).
 * @param a The first integer.
 * @param b The second integer.
 * @return true if a and b are coprime, false otherwise.
*/
bool checkCoprimivity(int a, int b);

/**
 * @brief Calculates the modular multiplicative inverse of a modulo n using the Extended Euclidean Algorithm.
 * @param a The number for which to find the inverse.
 * @param n The modulus value (n > 0).
 * @return The inverse (0 <= inverse < n), or -1 if the inverse does not exist (GCD(a, n) != 1).
*/
int invertible(int a, int n);

/**
 * @brief Calculates the modular sum or difference of a and b modulo n.
 * @param a The first integer.
 * @param b The second integer.
 * @param n The modulus.
 * @param plus_minus If true, performs (a + b) mod n; if false, performs (a - b) mod n.
 * @return The result in the range [0, n-1].
*/
int additionMudolar(int a, int b, int n, bool plus_minus);

/**
 * @brief Calculates the modular product or modular division of a and b modulo n.
 * @param a The first number.
 * @param b The second number (divisor if division is true).
 * @param n The modulus.
 * @param division If true, performs modular division (a * b^-1) mod n; if false, performs modular multiplication (a * b) mod n.
 * @return The result in the range [0, n-1], or -1 if the division is invalid.
*/
int multiplicationModular(int a, int b, int n, bool division);

/**
 * @brief Calculates modular exponentiation b^p mod n.
 * @param b The base.
 * @param p The integer power (exponent).
 * @param n The modulus.
 * @return The result b^p mod n, or -1 on error (e.g., n<=0).
 * @note This function uses the efficient 'Exponentiation by Squaring' method.
*/
int exponantialModular(int b, int p, int n); // Changed p to int for standard modular exponentiation

/**
 * @brief Solves the linear congruence a*x = b (mod n) for x.
 * @param a The coefficient of x.
 * @param b The result of the multiplication in the congruence.
 * @param n The modulus.
 * @return A solution x in the range [0, n-1], or -1 if no unique solution exists (or invertible(a,n) fails).
*/
int findTheX(int a, int b, int n);

int dividableBy(int a, int b);

#endif // MODULAR_H