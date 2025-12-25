#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "vectorOps.h"
#include "modular.h"      // Assumed existing

#define EPSILON_TEST 0.001

void test_modular_module() {
    printf("[TEST] Modular Module... ");
    
    // GCD
    assert(GCD(48, 18) == 6);
    assert(GCD(101, 103) == 1);
    
    // Modular Inverse (3 * 7 = 21 = 1 mod 10) -> Inverse of 3 mod 10 is 7
    assert(invertible(3, 10) == 7);
    assert(invertible(2, 10) == -1); // Even number mod Even number -> No inverse

    // Modular Arithmetic
    // (10 + 5) mod 12 = 3
    assert(additionMudolar(10, 5, 12, true) == 3); 
    
    printf("PASSED\n");
}

void test_vector_module() {
    printf("[TEST] Vector Ops Module... ");

    // Setup
    vector v1 = cnstVector(3);
    v1->val[0]=1; v1->val[1]=0; v1->val[2]=0; // i
    
    vector v2 = cnstVector(3);
    v2->val[0]=0; v2->val[1]=1; v2->val[2]=0; // j

    // Dot Product (i . j = 0)
    double dot = scalaricProduct(v1, v2);
    assert(fabs(dot) < EPSILON_TEST);

    // Cross Product (i x j = k)
    vector v3 = crossProduct(v1, v2);
    assert(fabs(v3->val[0] - 0) < EPSILON_TEST);
    assert(fabs(v3->val[1] - 0) < EPSILON_TEST);
    assert(fabs(v3->val[2] - 1) < EPSILON_TEST); // k is (0,0,1)

    // Addition (i + j = (1,1,0))
    vector v4 = addition(v1, v2, true);
    assert(fabs(v4->val[0] - 1) < EPSILON_TEST);
    assert(fabs(v4->val[1] - 1) < EPSILON_TEST);

    // Cleanup
    dcnstVector(v1);
    dcnstVector(v2);
    dcnstVector(v3);
    dcnstVector(v4);

    printf("PASSED\n");
}

int main() {
    printf("=== UNIT TEST RUNNER ===\n");
    test_modular_module();
    test_vector_module();
    printf("ALL MODULE UNIT TESTS PASSED.\n");
    return 0;
}