#ifndef SIMPLEX_H
#define SIMPLEX_H
#define EPSILON (0.00000001)

typedef struct simplex_t simplex_t;
struct simplex_t {
    int      m;     // Constraints
    int      n;     // Decisioin variables
    int*     var;   // [n + m] 0 .. n - 1 are nonbasic
    double** a;     // A [m][n + 1]
    double*  b;     // b [m]
    double*  x;     // x [n + 1]
    double*  c;     // c [n]
    double   y;     // y // result
};

void printConstraints(simplex_t*);

double simplex(int m, int n, double** a, double* b, double*c, double* x, double y);

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h);

void pivot(simplex_t* s, int row, int col);

int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);

int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var);

int select_nonbasic(simplex_t* s);

#endif
