#include <stdio.h>
#include "simplex.h"
#include "intopt.h"
#define EPSILON (0.0000001)
int main(){
    // 1
    simplex_t* simp = malloc(sizeof(simplex_t));
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

    int k  = init(simp, m, n, a, b, x, c, var);
    int nbi= select_nonbasic(simp); 
    pivot(simp, 0,1);
    printf("%d\n", k);

    // Free resources
    free(c);
    free(b);
    free(var);
    for (int i = 0; i != m; ++i)
        free(a[i]); // Free all sub pointers
    free(a); // Free other pointer
    free(x);
    fclose(fp);
    printConstraints(simp);
    delete_simp(simp);
}

int init(simplex_t* s, int m, int n, double** a, double* b, double* x, double* c, int* var) {
    int i, k; 
    init_simplex_t(s, m, n);
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
        c[col] = -c[col] / a[row][col];
    }

    for(i = 0; i < n; ++i){
        if(i != row) {
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    for (i = 0; i < n; ++i){
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
