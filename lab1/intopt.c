#include <stdio.h>
#include <stdlib.h>
#include "simplex.h"
#include "intopt.h"
#include "math.h"
#define EPSILON (0.000001)

// int global_array[10];
int main(){
    // 1
    //FILE* fp;
    //fp = fopen(stdin, "r");
    // Scan m && n
    int m,n;
    fscanf(stdin, "%d %d", &m, &n);
    //printf("%d, %d", m, n);
    // Scan c
    double* c;
    c = (double*) calloc(n, sizeof(double));
    //double c[n];
    for (int i = 0; i != n; ++i)
        fscanf(stdin, "%lf", &c[i]);

    double** a = (double**) calloc(m, sizeof(double* ));
    for(int i = 0; i != m; ++i)
        a[i] = (double*) calloc(n + 1, sizeof(double));

    // Scan a
    for (int i = 0; i != m; ++i)
        for (int k = 0; k != n; ++k)
            fscanf(stdin, "%lf", &a[i][k]);

    double* b = (double*) calloc(m, sizeof(double));
    // Scan b
    for (int i = 0; i != m; ++i)
        fscanf(stdin, "%lf", &b[i]);

    //double x[n + 1];
    double* x = (double*) calloc(n + 1, sizeof(double)); // x are zeroes
    double y = 0;
    double res = simplex(m, n, a, b, c, x, y);
    printf("%lf", res);
    // Free resources
    free(c);
    free(b);
    for (int i = 0; i != m; ++i)
        free(a[i]); // Free all sub pointers
    free(a); // Free other pointer
    free(x);
    fclose(stdin);
}

int init(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x, double y, int* var) {
    int i, k; 
    s->m = m;
    s->n = n;
    s->a = a;
    s->b = b;
    s->c = c;
    s->x = x;
    s->y = y;
    s->var = var;

    
    printf("in init. \n");
    printConstraints(s);
    if (s->var == NULL){
        printf("Var == NULL\n");
        s->var = calloc((m + n + 1) , sizeof(int));
        for (int i = 0; i < m + n; ++i){
            s->var[i] = i;
        }
    }
    for (int i =0 ; i <n + m; ++i)
        printf("init : ");
        printf("%d", s->var[i]);
    for (k = 0, i = 1; i < m; ++i){
        if (b[i] < b[k]){
            k = i;
        }
    }
    return k;
}

int select_nonbasic(simplex_t* s){
    for(int i = 0; i < s->n; ++i){
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
    int m = s->m;
    int n = s->n;
    for (int i = m + n; i > n; --i){
        s->var[i] = s->var[i - 1];
    }
    // add x[m+n] to each constraints
    s->var[n] = m + n;
    n = n + 1;
    for (int i = 0; i < m; ++i) {
        s->a[i][n - 1] = -1;
    }
    s->x = (double*) calloc(m + n, sizeof(double));
    s->c = (double*) calloc(n, sizeof(double));
    s->c[n - 1] = -1;
    s->n = n;
    pivot(s, k, n - 1);
}
int initial(simplex_t* s, int m, int n, double** a, double* b, double* c, double* x , double y, int* var) {
    int i,j,k;
    double w;
    k = init(s, m, n, a, b, c, x, y, var);
    if (b[k] >= 0) {
        return 1;
    }
    prepare(s,k);
    printf("After prepare.\n");
    printConstraints(s);
    n = s->n;
    s->y = xsimplex(m, n, s->a, s->b, s->c, s->x, 0, s->var, 1);
    //printf("s->y == %lf\n", s->y);
    printf("\n");
    for (int l =0 ; l <n + m; ++l)
        printf("%d", s->var[l]);
    for(i = 0; i < s->m + s->n; ++i) {
        if (s->var[i] == m + n - 1) {  // CHANGED HERE
            if (fabs(s->x[i]) > EPSILON) {
                free(s->x);
                free(s->c);
                return 0;
            } else {
                break;
            }
        }
    }
    printf("feasible. \n");
    //printConstraints(s);
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
        k = s->var[i];
        s->var[i] = s->var[n - 1];
        s->var[n - 1] = k;
        for (k = 0;  k < m; ++k){
            w = s->a[k][n-1];
            s->a[k][n -1] = s->a[k][i];
            s->a[k][i] = w;
        }
    } else {
        // x[n][m]  is nonbasic and last, forget it.
    }
    free(s->c);
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
            if (s->var[n + j] == k){
                break;
            }
        }
        s->y = s->y + s->c[k] * s->b[j];
        for (i = 0; i < n; ++i){
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }
next_k:;
    }

    for (i = 0;  i < n; ++i){
        s->c[i] = t[i];
    }
    free(t);
    free(s->x);
    return 1;
}

double xsimplex(int m, int n, double** a, double* b, double* c, double* x, double y, int* var, int h){
    //printf("xsimplex\n");
    simplex_t* s = malloc(sizeof(simplex_t));
    if (!initial(s, m, n, a, b, c, x, y, var)) {
        free(s->var);
        free(s);
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
            free(s);
            return INFINITY;
        }
        pivot(s, row, col);
    }

    if (h == 0) {
        //printf("h == 0 \n");
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
        free(s->var);
    } else {
        for (int i = 0; i < n; ++i){
            x[s->var[i]] = 0;
        }
        for (int i = n; i < n+m; ++i){
            x[s->var[i]] = s->b[i - n];
        }
    }
    double ret = s->y;
    free(s);
    return ret;
}
double simplex(int m, int n, double** a, double* b, double* c, double* x, double y) {
    return xsimplex(m, n, a, b, c, x, y, NULL, 0);
}
