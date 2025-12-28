#ifndef CALCULUS_H
#define CALCULUS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double double_integral(double (*func)(double,double),
                       double a, double b, double c,  double d);

double intergal(double (*func)(double), double a, double b);

double derivative(int var, double (function)(double,double), double x, double y);

double L(double (*target)(double,double), double (*constraint)(double,double),
         double x, double y, double l);

double derivative(int var, double (function)(double,double), double x, double y);

void gradient_L(double (*target)(double,double),
                  double (*constraint)(double,double),
                  double x, double y, double l,
                  double *dL_dx, double *dL_dy, double *dL_dl);

double find_optimal(double (*target)(double,double), double (*constraint)(double,double));
#endif