#include <stdio.h>
#include "simplex.h"
#include "intopt.h"
#include "math.h"
#define EPSILON (0.0000001)
int main(){
    // 1
    FILE* fp;
    fp = fopen("constraints", "r");
    // Scan m && n
    int m,n;
    fscanf(fp, "%d %d", &m, &n);

    // Scan c
    double* c; 
    c = (double*) malloc(sizeof(double) * (n));
    for (int i = 0; i != n; ++i)
        fscanf(fp, "%lf", &c[i]);

    double** a = (double**) calloc(m, sizeof(double* ));
    for(int i = 0; i != m; ++i)
        a[i] = (double*) calloc(n, sizeof(double));

    // Scan a
    for (int i = 0; i != m; ++i)
        for (int k = 0; k != n; ++k)
            fscanf(fp, "%lf", &a[i][k]);

    double* b = (double*) malloc(sizeof(double) * (m));
    // Scan b
    for (int i = 0; i != n; ++i)
        fscanf(fp, "%lf", &b[i]);

    int* var  = (int*) calloc(n + m, sizeof(int)); // var are zeroes
    double* x = (double*) calloc(n + 1, sizeof(double)); // x are zeroes
    double y = 0;
    double res = simplex(m, n, a, b, c, x, y);
    printf("%lf", res);
    // Free resources
    free(c);
    free(b);
    free(var);
    for (int i = 0; i != m; ++i)
        free(a[i]); // Free all sub pointers
    free(a); // Free other pointer
    free(x);
    fclose(fp);
}

int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    int i, k; 
    init_simplex_t(s, m, n);
    s->y = y;
    // Assign c
    for (int l = 0; l != n; ++l)
        s->c[l] = c[l];
    // Assign a
    for (int l = 0; l != m; ++l)
        for (int p = 0; p != n; ++p)
            s->a[l][p] = a[l][p];
    // Assign b
    for (int l = 0; l != n; ++l)
        s->b[l] = b[l];

    for (i = 0; i < m+ n; ++i)
        s->var[i] = i;

    for (k = 0, i = 1; i < m; ++i){
        if (b[i] < b[k])
            k = i;
    }
    return k;
}

int select_nonbasic(simplex_t* s){
    for(int i = 0; i != s->n; ++i){
        if (s->c[i] > EPSILON) {
            return i;
        }
    }
    return -1;
}

void pivot(simplex_t* s, int row, int col){
    double** a = s->a;
    double*  b = s->b;
    double*  c = s->c;
    int      m = s->m;
    int      n = s->n;
    int      i,j,t;
    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n+row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];

    for(i = 0; i < n; ++i){
        if (i != col) {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }

    }
    // HERE
    c[col] = -c[col] / a[row][col];
    for(i = 0; i < m; ++i){
        if(i != row) {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    for (i = 0; i < m; ++i){
        if( i != row) {
            for(j = 0; j < n; ++j){
                if(j != col) {
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }

    for (i = 0; i < m; ++i) {
        if( i != row) {
            a[i][col] = - a[i][col] / a[row][col];
        }
    }

    for (i = 0; i < n; ++i) {
        if(i != col) {
            a[row][i] = a[row][i] / a[row][col];
        }
    }

    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col]; 
}

int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var) {
    int i,j,k;
    double w;
    k = init(s, m, n, a, b, c, x, y, var);
    return 1;
    /*
       if (b[k] >= 0) {

       }
       */
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h){
    simplex_t* s = malloc(sizeof(simplex_t));
    // No c in initial?
    if (!initial(s, m, n, a, b, c, x, y, var)) {
        delete_simp(s);
        return NAN; 
    }
    int row, col;
    while((col = select_nonbasic(s)) >= 0){
        row = -1; 
        for(int i = 0; i < m; ++i){
            if (a[i][col] > EPSILON && (row < 0 || (b[i] / a[i][col]) < (b[row] / a[row][col]))){
                row = i;
            }
        }
        if (row < 0 ){
            free(s->var);
            return INFINITY;
        }
        pivot(s, row, col);
    }

    if (h == 0) {
        for (int i = 0; i < n; ++i){
            if (s->var[i] < n){
                x[s->var[i]] = 0;
            }
        }

        for(int i = 0; i < m; ++i){
            if (s->var[n + 1] < n) {
                x[s->var[n + i]] = s->b[i];
            }
        }
    } else {
        for (int i = 0; i < n; ++i){
            x[s->var[i]] = 0;
        }
        for (int i = n; i < n+m; ++i){
            x[s->var[i]] = s->b[i - n];
        }
    }
    double ret = s->y;
    delete_simp(s);
    return ret;
}
double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}
