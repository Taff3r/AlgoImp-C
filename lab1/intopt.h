#ifndef INTOPT_H
#define INTOPT_H
#include "simplex.h"
int init(simplex_t*, int m, int n, double** a, double* b, double* c, double * x, double y, int* var);
int select_nonbasic(simplex_t*);
void pivot(simplex_t*, int, int);
double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);
int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int*);
double simplex(int m, int n, double** a, double* b, double* c, double* x, double y);
void prepare(simplex_t* s, int k);
#endif
