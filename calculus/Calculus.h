#ifndef CALCULUS_H
#define CALCULUS_H

// basic functions

double LinearFunc(double x, double m, double b);

double singleVarFunc(double x, char *funcStr);

double multiVarFunc(int varCount, double *varList, char *funcStr);

double derivativeSingleVar(double x, char *funcStr, double h);

double derivativeMultiVar(int varCount, double *varList, int varIndex, char *funcStr, double h);
#endif