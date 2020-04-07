#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <stdlib.h>
#include <stdio.h>
typedef struct {
    int      m;
    int      n;
    int*     var;  //[n + m];
    double** a;    //[m][n + 1];
    double*  b;    //[m];
    double*  x;    //[n + 1];
    double*  c;    //[n];
    double   y;
} simplex_t;
// Allocate memory for the simplex.
void init_simplex_t(simplex_t* s, int  m, int n);

// Free allocated memory for the simplex
void delete_simp(simplex_t* s);

// Print the constraints of the simplex.
void printConstraints(simplex_t* simp);

#endif
