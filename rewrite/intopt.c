#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simplex.h"

int main() {
    int m, n;
    fscanf(stdin, "%d%d", &m, &n);

    // Allocate
    double*  c = calloc(n, sizeof(double));
    double** a = malloc(m * sizeof(double*));
    double*  b = calloc(m, sizeof(double));
    double*  x = calloc(n + 1, sizeof(double));;

    int i, k;
    for (i = 0; i < m; ++i)
        a[i] = calloc(n + 1, sizeof(double));

    // Scan
    for (i = 0; i < n; ++i)
        fscanf(stdin, "%lf", &c[i]);

    for (i = 0; i < m; ++i)
        for (k = 0; k < n; ++k)
            fscanf(stdin, "%lf", &a[i][k]);

    for (i = 0; i < m; ++i)
        fscanf(stdin, "%lf", &b[i]);

    // Solve    
    int y = 0;
    double sol = simplex(m, n, a, b, c, x, y);
    printf("%lf", sol);

    // Free 
    free(c);
    for(i = 0; i < m; ++i)
        free(a[i]);
    free(a);
    free(b);
    free(x);
}

void printConstraints(simplex_t* simp){
    printf("max z = ");
    for (int i = 0; i != simp->n; ++i){
        printf("%+fx%d", simp->c[i], i);
        if ( i + 1 != simp->n){
            printf(" + ");
        }			
    }
    printf("\n");
    for (int i = 0; i != simp->m; ++i){
        for(int k = 0; k != simp->n; ++k){
            printf("%+fx%d", simp->a[i][k], k);
            if ( k + 1 != simp->n){
                printf(" + ");
            }
        }	
        printf(" \u2264 %f\n", simp->b[i]);
    }
}

double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h) {
    simplex_t* s = malloc(sizeof(simplex_t));
    if(!initial(s, m, n, a, b, c, x, y, var)) {
        free(s->var);
        free(s);
        return NAN;
    }
    int row, col, i;
    while((col = select_nonbasic(s)) >= 0){
        row = -1;
        for(i = 0; i < m; ++i) {
            if (a[i][col] > EPSILON && (row < 0 || b[i] / a[i][col] < b[row] / a[row][col])){
                row = i;
            }
        }
        if (row < 0) {
            free(s->var);
            free(s);
            return INFINITY;
        }
        pivot(s, row, col);
    }

    if (h == 0) {
        for (i = 0; i < n; ++i){
            if(s->var[i] < n){
                x[s->var[i]] = 0;
            }
        }

        for (i = 0; i < m; i += 1) {
            if (s->var[n + 1] < n){
                x[s->var[n + i]] = s->b[i];
            }
        }

        free(s->var);
    } else {
        for (i = 0; i < n; ++i) {
            x[s->var[i]] = 0;
        }

        for (i = n; i < n + m; ++i){
            x[s->var[i]] = s->b[i - n];
        }
    }
    double ret = s->y;
    free(s);
    return ret;
}
int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    s->m   = m;
    s->n   = n;
    s->a   = a;
    s->b   = b;
    s->c   = c;
    s->x   = x;
    s->y   = y;
    s->var = var;

    int i, k;
    if (s->var == NULL) {
        s->var = calloc(m + n + 1, sizeof(int));
        for (i = 0; i < m + n; ++i) {
            s->var[i] = i;
        }
    }

    for (k = 0, i = 1; i < m; ++i) {
        if (b[i] < b[k]) {
            k = i;
        }
    }

    return k;
}

int select_nonbasic(simplex_t* s) {
    int i;
    for (i = 0; i < s->n; ++i) {
        if (s->c[i] > EPSILON) {
            return i;
        }
    }
    return -1;
}

void pivot(simplex_t* s, int row, int col) {
    // Why auto?
    double** a = s->a;
    double*  b = s->b;
    double*  c = s->c;
    int      m = s->m;
    int      n = s->n;
    int      i,j,t;

    t = s->var[col];
    s->var[col] = s->var[n + row];
    s->var[n + row] = t;
    s->y = s->y + c[col] * b[row] / a[row][col];

    for (i = 0; i < n; ++i) {
        if (i != col) {
            c[i] = c[i] - c[col] * a[row][i] / a[row][col];
        }
    }
    c[col] = - c[col] / a[row][col];
    for(i = 0; i < m; ++i) {
        if(i != row){
            b[i] = b[i] - a[i][col] * b[row] / a[row][col];
        }
    }

    for(i = 0; i < m; ++i) {
        if (i != row) {
            for(j = 0; j < n; ++j) {
                if(j != col){
                    a[i][j] = a[i][j] - a[i][col] * a[row][j] / a[row][col];
                }
            }
        }
    }

    for(i = 0; i < m; ++i) {
        if(i != row) {
            a[i][col] = -a[i][col] / a[row][col];
        }
    }

    for(i = 0; i < n; ++i) {
        if (i != col) {
            a[row][i] = a[row][i] / a[row][col];
        }
    }
    b[row] = b[row] / a[row][col];
    a[row][col] = 1 / a[row][col];
}

int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
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
    
    // Using Skeppstedt's loop
    for (i = 0; i < s->m + s->n; i += 1) {
        if (s->var[i] == m + n - 1) {
            if (fabs(s->x[i]) > EPSILON) {
                free(s->x);
                free(s->c);
                return 0; // Infeasible
            } else {
                break;
            }
        } 
        printf("i = %d, m = %d, n = %d k = %d\n", i, m, n, k);
    }

    if (i >= n) {
        for(j = k = 0; k < n; k += 1) {
            printf("i = %d, s->n = %d, n = %d, s->m = %d, m = %d, k = %d, j = %d \n", i, s->n, n, s->m, m, k, j);
            if (fabs(s->a[i - n][k]) > fabs(s->a[i - n][j])) {
                j = k;
            }
        }
        pivot(s, i - n, j);
        i = j;
    }

    if (i < n - 1) {
        k = s->var[i];
        s->var[i] = s->var[n - 1];
        s->var[n - 1] = k;
        for (k = 0; k < m; ++k) {
            w = s->a[k][n - 1];
            s->a[k][n - 1] = s->a[k][i];
            s->a[k][i] = w;
        }
    } else {
        // x_n+m is nonbasic and last. Forget it.
    }

    free(s->c);
    s->c = c;
    s->y = y;
    for (k = n - 1; k < n + m - 1; ++k) {
        s->var[k] = s->var[k + 1];
    }
    n = s->n = s->n - 1;
    double* t = calloc(n, sizeof(double));
    for (k = 0; k < n; ++k) {
        for (j = 0; j < n; ++j) {
            if (k == s->var[j]) {
               t[j] += s->c[k];
               goto next_k;
            }
        }

        for (j = 0; j < m; ++j) {
            if (s->var[n + j] == k) {
                break;
            }
            s->y = s->y + s->c[k] * s->b[j];
        }

        for (i = 0; i < n; ++i) {
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }
        next_k:;
    }

    for(i = 0; i < n; ++i) {
        s->c[i] = t[i];
    }
    free(t);
    free(s->x);
    return 1;
}

void prepare(simplex_t* s, int k) {
    int m = s->m;
    int n = s->n;
    int i;
    for(i = m + n; i > n; --i) {
        s->var[i] = s->var[i - 1];
    }
    s->var[n] = m + n;
    n = n + 1;
    for (i = 0; i < m; ++i){
        s->a[i][n - 1] = -1;
    }
    s->x = calloc(m + n, sizeof(double));
    s->c = calloc(n, sizeof(double));
    s->c[n - 1] = -1;
    s->n = n;
    pivot (s,k, n-1);
}
