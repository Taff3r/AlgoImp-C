#include <stdio.h>
#include <stdlib.h>
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
    for (int i = 0; i != m; ++i)
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
    // Assign b m or n?
    for (int l = 0; l != m; ++l)
        s->b[l] = b[l];
    
    for (i = 0; i < m+ n; ++i)
        s->var[i] = i;
    if (s->var == NULL){
        s->var = calloc(m + n + 1, sizeof(int));
        for (i = 0; i < m + n; ++i){
            s->var[i] = i;
        }
    }
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
void prepare(simplex_t* s, int k) {
    printf("In prepare");
    int m = s->m;
    int n = s->n;
    for (int i = m + n; i > n; --i){
        s->var[i] = s->var[i - 1];
    }
    // add x[m+n] to each constraints
    s->var[n] = m + n;
    ++n;
    for (int i = 0; i < m; ++i) {
        s->a[i][n - 1] = -1;
    }
    // Maybe do frees here?
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n - 1] = -1;
    s->n = n;
    pivot(s, k, n - 1);
}
int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var) {
    int i,j,k;
    double w;
    k = init(s, m, n, a, b, c, x, y, var);
    printConstraints(s);
    if (b[k] >= 0) {
        return 1;
    }
    prepare(s,k);
    n = s->n;
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
    printf("s->y == %lf\n", s->y);
    for(i = 0; i < s->m + s->n; ++i) {
        if (s->var[i] == m + n - 1) {  // CHANGED HERE
            if (fabs(s->x[i]) > EPSILON) {
                printf("Original system is infeasible\n");
                free(s->x);
                free(s->c);
                return 0;
            } else 
                break;
        }
    }
    printf("feasible. \n");
    printConstraints(s);
    if (i >= n) {
        for (j = k = 0; k < n; ++k){
            if (fabs(s->a[i - n][k]) > fabs(s->a[i - n][j])){
                j = k;
            }
        }
        pivot(s, i-n, j);
        i = j;
    }

    if (i < n - 1) {
        for (j = k; k < n; ++k){
            k = s->var[i];
            s->var[i] = s->var[n - 1];
            s->var[n - 1] = k;
            for (k = 0;  k < m; ++k){
                w = s->a[k][n-1];
                s->a[k][n -1] = s->a[k][i];
                s->a[k][i] = w;
            }
        }
    } else {
        // x[n][m]  is nonbasic and last, forget it.
    }
    free(s->c);
    // Realloc?
    s->c = c;
    s->y = y;
    for (k = n -1; k < n + m - 1; ++k){
        s->var[k] = s->var[k + 1];
    }
    n = s->n = s->n - 1;
    // nedd malloc?
    double* t = calloc (n, sizeof(double));
    for (k = 0; k < n; ++k){
        for (j = 0; j < n; ++j){
            if (k == s->var[j]){
                t[j] = t[j] + s->c[k];
                goto next_k;
            }
        }
        for (j = 0; j < m; ++j){
            if (s->var[n + j] == k)
                break;
        }
        s->y = s->y + s->c[k] * s->b[j];
        for (i = 0; i < n; ++i){
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }
        next_k:;
    }

    for (i = 0;  i < n; ++i)
        s->c[i] = t[i];
    free(t);
    free(s->x);
    return 1;
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h){
    printf("xsimplex\n");
    simplex_t* s = malloc(sizeof(simplex_t));
    if (!initial(s, m, n, a, b, c, x, y, var)) {
        free(s->var);
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
        printf("h == 0 \n");
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
    return s->y;
}
double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}
