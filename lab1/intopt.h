#ifndef INTOPT_H
#define INTOPT_H
#include "simplex.h"
int init(simplex_t*, int, int, double**, double*, double*, double*, int*);
int select_nonbasic(simplex_t*);
void pivot(simplex_t*, int, int);
#endif
