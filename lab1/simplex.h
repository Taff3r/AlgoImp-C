#include <stdlib.h>
typedef struct {
    int      m;
    int      n;
    int*     var;  //[n + m];
    double** a;    //[m][n + 1];
    double*  b;    //[m];
    double*  x;    //[n + 1];
    double*  c;    //[n];
    double  y;
} simplex_t;

void init_simplex_t(simplex_t* s, int  m, int n, int var[]){
    s->m   = m;
    s->n   = n;
    s->var = (int*) malloc(sizeof(int) * (n + m));
    s->a   = (double**) malloc(sizeof(double*) * m);
    for(int i = 0; i != m; ++i)
        s->a[i] = (double*) malloc(sizeof(double) * n);

    s->b   = (double*) malloc(sizeof(double) * (m));
    s->c   = (double*) malloc(sizeof(double) * (n));
    s->x   = (double*) malloc(sizeof(double) * (n + 1));
}
