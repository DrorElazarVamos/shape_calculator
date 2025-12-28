#include <stdlib.h>
#include <math.h>

double intergal(double (*func)(double), double a, double b){
    double dx = 0.0001, res = 0.0;
    for (double i = a; i<b; i+dx) res += func(i)*dx;
    return res;
}

double double_integral(double (*func)(double,double),
                       double a, double b, double c,  double d){
        double dx = 0.0001, dy = 0.0001;
        double res =0.0;
        for (unsigned int i = a; i < b; i+dx){
            for (unsigned int j = c; j<d; j+dy) res += func(i,j)*(dx*dy);
        }

        return res;
   }