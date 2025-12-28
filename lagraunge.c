#include <stdio.h>
#include <math.h>

#define DELTA 1e-12

double L(double (*target)(double,double), double (*constraint)(double,double),double x, double y, double l){
    return target(x,y) - l*constraint(x,y);
}

double derivative(int var, double (function)(double,double), double x, double y){
    double df = 0.0;

    if (var == 0) df = function(x+DELTA,y) - function(x,y);
    else if (var == 1) df = function(x,y+DELTA) - function(x,y);
    return df/DELTA;
}


void gradient_L(double (*target)(double,double),
                  double (*constraint)(double,double),
                  double x, double y, double l,
                  double *dL_dx, double *dL_dy, double *dL_dl){
    *dL_dx = derivative(0,target,x,y) - l*derivative(0,constraint,x,y);
    *dL_dy = derivative(1,target,x,y) - l*derivative(1,constraint,x,y);
    *dL_dl = (-1)*(constraint)(x,y); 

}


double find_optimal(double (*target)(double,double), double (*constraint)(double,double)){
    //init the variable
    double learning_curve = 0.01;
    unsigned int reps = 10000;

    double x = 0.0, y = 0.0, l = 0.0;
    double dL_dx,dL_dy,dL_dl;

    for (unsigned int i = 0 ; i < reps; i++){
        gradient_L(target,constraint,x,y,l,&dL_dx,&dL_dy,&dL_dl);
        if(fabs(dL_dx) < 0.0001 && fabs(dL_dy) < 0.0001 && fabs(dL_dl) < 0.0001){
            printf("Converged at iteration %d\n", i);
            break;
        }

        x = x + learning_curve * dL_dx;
        y = y + learning_curve * dL_dy;
        l = l + learning_curve * dL_dl;
    
    }
    return target(x,y);
}