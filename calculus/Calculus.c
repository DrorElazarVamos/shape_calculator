#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Calculus.h"

double LinearFunc(double x, double m, double b){
    if (m==0) return b;
    return m * x + b;
}

double singleVarFunc(double x, char *funcStr){
    if (!funcStr){
        fprintf(stderr, "Error: function string is NULL.\n");
        exit(EXIT_FAILURE);
    }

    unsigned short eq = 0;

    while (*funcStr){
        while (funcStr != eq) {
            funcStr++;
            continue;
        }

        if (*funcStr == 'x'){
            funcStr++;
            continue;
    }

}

double multiVarFunc(int varCount, double *varList, char *funcStr);

double derivativeSingleVar(double x, char *funcStr, double h);

double derivativeMultiVar(int varCount, double *varList, int varIndex, char *funcStr, double h);