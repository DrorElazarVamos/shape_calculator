#include "modular.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h> // Included for standard I/O if needed

// --- GCD Implementation ---

int GCD(int a, int b) {
    // Ensure both numbers are non-negative for the standard algorithm
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    
    // Check for modulus 0, which is often a logic error in modular context
    if (a == 0 && b == 0) return 0; // Or handle as an error if appropriate
    
    // Euclidean Algorithm
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool checkCoprimivity(int a, int b){
    return (GCD(a, b) == 1);
}

// --- Modular Multiplicative Inverse (Invertible) ---

int invertible(int a, int n){
    // Critical check: inverse only exists if GCD(a, n) = 1
    if (GCD(a,n)!=1){
        return -1;
    }

    // Normalize 'a' to the range [0, n-1] for clean loop operation
    a = a % n;
    if (a < 0) {
        a += n;
    }

    // variables for Extended Euclidean Algorithm
    int n0 = n; // holds the OG value of n
    int x0 = 0;
    int x1 = 1;

    // Extended Euclidean Algorithm
    while(a > 0){
        int q = n / a;
        int t = n - a * q; // t = n % a
        n = a;
        a = t;
        
        t = x0 - q * x1;
        x0 = x1;
        x1 = t;
    }

    // Final check: if n is 1, x0 is the inverse
    if (n == 1){
        if (x0 < 0){
            x0 += n0;
        }
        return x0;
    }

    return -1;  
}

// --- Modular Addition/Subtraction ---
//

int additionMudolar(int a, int b, int n, bool plus_minus){
    // Logic Issue Fix: Ensure result is in [0, n-1] (C's % can return negative)
    // Use long long to prevent potential overflow of a+b before modulo
    long long result;
    
    if (plus_minus == true){
        result = (long long)a + b;
    } else {
        result = (long long)a - b;
    }

    // Normalize result (handle negative modulo result)
    result = result % n;
    if (result < 0){
        result += n;
    }
    
    return (int)result;
}

// --- Modular Multiplication/Division ---

int multiplicationModular(int a, int b, int n, bool division){
    // Logic Issue Fix: Modular Division is a * b^-1 mod n
    if (division == true){
        int invertibleB = invertible(b, n);
        
        if (invertibleB == -1) {
            // Cannot divide because b is not invertible mod n
            return -1; 
        }

        // Modular division is multiplication by the inverse
        return multiplicationModular(a, invertibleB, n, false);

    } else {
        // Logic Issue Fix: The original multiplication logic (a%n)*(b*n) was wrong
        // Correct modular multiplication (a * b) mod n
        // Use long long to prevent overflow of a * b
        long long result = (long long)a * b;
        
        // Normalize result (handle negative modulo result, though less likely here)
        result = result % n;
        if (result < 0){
            result += n;
        }
        return (int)result;
    }
}

// --- Modular Exponentiation (Corrected to use Exponentiation by Squaring) ---

int exponantialModular(int b, int p, int n){
    // Logic Issue Fix: p must be an integer, and the algorithm must prevent overflow
    if (n <= 0) return -1; // Invalid modulus
    if (p < 0) return -1; // Cannot handle negative exponents easily without inverse

    long long base = b % n;
    long long result = 1;
    
    while (p > 0) {
        // If p is odd, multiply the result with the base
        if (p & 1) { 
            result = (result * base) % n;
        }
        
        // p must be even now (for the next step)
        p = p >> 1; // p = p / 2
        
        // Square the base for the next iteration
        base = (base * base) % n;
    }

    return (int)result;
}

// --- Congruence Solving (a*x = b mod n) ---

int findTheX(int a, int b, int n){
    // Find a^-1 mod n
    int invertibleA = invertible(a,n);
    if (invertibleA == -1){
        // No unique solution exists if a is not invertible mod n
        return -1;
    }
    
    // The solution is x = b * a^-1 mod n
    // Logic Issue Fix: The result must be taken modulo n, and intermediate multiplication needs long long
    return multiplicationModular(b, invertibleA, n, false);
}

// --- General Division (Removed from modular context, but included for completeness) ---

/**
 * @brief Checks if a is divisible by b.
 * @param a The dividend.
 * @param b The divisor.
 * @return The quotient (a/b) if a is divisible by b, or -1 otherwise.
*/
int dividableBy(int a, int b){
    if (b == 0) return -1; // Cannot divide by zero
    
    // Standard division check
    if (a % b == 0){
        return a / b;
    } else {
        return -1;
    }
}