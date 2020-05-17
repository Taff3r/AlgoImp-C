#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "simplex.h"
#include "node_set.h"
#include "bnb.h"

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
    double sol = intopt(m, n, a, b, c, x);
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
            if (s->var[n + i] < n){
                x[s->var[n + i]] = s->b[i];
                //x[s->var[n]] = s->b[i];
            }
        }
        free(s->var);
    } else {
        for (i = 0; i < n; ++i) {
            x[i] = 0;
        }

        for (i = n; i < n + m; ++i){
            x[i] = s->b[i - n];
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
    }

    if (i >= n) {
        for(j = k = 0; k < n; k += 1) {
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
        }

        s->y = s->y + s->c[k] * s->b[j];
        for (i = 0; i < n; ++i) {
            t[i] = t[i] - s->c[k] * s->a[j][i];
        }
next_k:;
    }
    // Why not just copy contents?
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

node_t* initial_node(int m, int n, double** a, double* b, double* c) {
    node_t* p = malloc(sizeof(node_t));
    p->m = m; 
    p->n = n;
    p->a = malloc((m + 1) * sizeof(double*));
    p->b = calloc(m + 1, sizeof(double));
    p->c = calloc(n + 1, sizeof(double));
    p->x = calloc(m + n + 1, sizeof(double));
    p->min = calloc(n, sizeof(double));
    p->max = calloc(n, sizeof(double));
    
    int i;
    for(i = 0; i < p->m + 1; ++i) {
        p->a[i] = calloc((p->n + 1) , sizeof(double));
    }
    // Double check
    //memcpy(p->a, a, sizeof(double*));

    // NEW
    for(i = 0; i < m; ++i){
        memcpy(p->a[i], a[i], (p->n) * sizeof(double));
    }
    memcpy(p->b, b, p->n * sizeof(double));
    memcpy(p->c, c, p->n * sizeof(double));
    //p->a = a;
    //p->b = b;
    //p->c = c;

    for(i = 0; i < p->n; ++i) {
        p->min[i] = -INFINITY;
        p->max[i] = INFINITY;
    }
    return p;
}


node_t* extend(node_t* p, int m, int n, double** a, double* b, double* c, int k, double ak, double bk) {
    node_t* q = malloc(sizeof(node_t));
    //node_t* q = initial_node(m,n,a,b,c);
    q->k = k;
    q->ak = ak;
    q->bk = bk;
    // NOTE NOT IN PSEUDO CODE
    //q->h = p->h;
    if (ak > 0 && p->max[k] < INFINITY) {
        q->m = p->m;
    } else if(ak < 0 && p->min[k] > 0) {
        q->m = p->m;
    } else {
        q->m = p->m + 1;
    }

    q->n = p->n;
    q->h = -1;

    q->a = malloc((q->m + 1) * sizeof(double*));
    q->b = calloc(q->m + 1, sizeof(double));
    q->c = calloc(q->n + 1, sizeof(double));
    q->x = calloc(q->n + 1, sizeof(double));
    q->min = calloc(n + 1, sizeof(double));
    q->max = calloc(n + 1, sizeof(double));

    int i;
    for(i = 0; i < q->m + 1; ++i) {
        q->a[i] = calloc((q->n + 1), sizeof(double));
    }

    // double check
    memcpy(q->min, p->min, (p->n) * sizeof(double));
    memcpy(q->max, p->max, (p->n) * sizeof(double));
    //q->min = p->min;
    //q->max = p->max;

    // Copy m first forw of paramter a to q->a. Each element and not only pointers
    for (i = 0; i < m; ++i) {
        memcpy(q->a[i], a[i], (n + 1) * sizeof(double));
    }
    memcpy(q->b, b, m * sizeof(double));
    memcpy(q->c, c, n * sizeof(double));

    if(ak > 0) {
        if (q->max[k] == INFINITY || bk < q->max[k]) {
            q->max[k] = bk;
        }
    } else if (q->min[k] == -INFINITY || -bk > q->min[k]) {
        q->min[k] = -bk;
    }

    int j;
    for (i = m, j = 0; j < n; ++j){
        if(q->min[j] > -INFINITY) {
            q->a[i][j] = -1;
            q->b[i]    = -q->min[j];
            ++i;
        }
        if (q->max[j] < INFINITY) {
            q->a[i][j] = 1;
            q->b[i] = q->max[j];
            ++i;
        }
    }
    return q;
}

int is_integer(double* xp) {
    double x = *xp;
    double r = lround(x);
    if (fabs(r - x) < EPSILON) {
        *xp = r;
        return 1;
    } else {
        return 0;
    }
}

int integer(node_t* p) {
    for(int i = 0; i < p->n; ++i){
        if (!is_integer(&p->x[i])) {
            return 0;
        }
    }
    return 1;
}

void bound(node_t* p, NodeSet* h, double* zp, double* x) {
    if (p->z > *zp) {
        *zp = p->z;
        // double check copy each element of p->x to x // save best x
        memcpy(x, p->x, (p->n + 1) * sizeof(double)); // TODO Make sure this is the correct size
        // remove and delete all nodes in q in h with q->z < p->z
        // TODO Double check that this works
        int sz = h->size;
        for(int i = 0; i < sz; ++i){
            node_t* q = get(h);
            if(q->z < *zp){
                free(q->min);
                free(q->max);
                free(q);
            } else {
                put(h, q);
            }
        }
    }
}

double intopt(int m, int n, double** a, double* b, double* c, double* x) {
    node_t* p = initial_node(m, n, a, b, c);
    NodeSet* h = initNodeSet();
    put(h,p);
    double z = -INFINITY;
    p->z = simplex(p->m, p->n, p->a, p->b, p->c, p->x, 0); // q->x???
    if (integer(p) || !isfinite(p->z)) {
        z = p->z;

        // Why is this here if we are returning afterwards anyways? Just to print the sol.?
        if(integer(p)){
            x = p->x;
            //memcpy(x, p->x, n * sizeof(double)); // TODO Get correct size
        }
        free(p->max);
        free(p->min);
        free(p->x);
        free(p->b);
        for(int i = 0; i < p->m + 1; ++i) {
            free(p->a[i]);
        }
        free(p->a);
        free(p->c);
        deleteNodeSet(h);
        return z;
    }
    branch(p, z);
    while(!isEmpty(h)) {
        // take p from h 
        p = get(h);
        // fuckywucky pseudocode
        succ(p, h, m, n, a, b, c, p->h, 1, floor(p->xh), &z, x);
        succ(p, h, m, n, a, b, c, p->h, -1, -ceil(p->xh), &z, x);
        free(p->max);
        free(p->min);
        free(p); // free pointers in p aswell?
    }
    deleteNodeSet(h);
    if (z == -INFINITY) {
        return NAN;
    } 
    return z;
}
// h is acctually a set lol
void succ(node_t* p, NodeSet* h, int m, int n, double** a, double* b, double* c, int k, double ak, double bk, double* zp, double* x) {
    node_t* q = extend(p, m, n, a, b, c, k, ak, bk);
    if (q == NULL) {
        return;
    }
    q->z = simplex(q->m, q->n, q->a, q->b, q->c, q->x, 0);
    if(isfinite(q->z)){
        if (integer(q)) {
            bound(q, h, zp, x);
        } else if (branch(q, *zp)) {
            // add q to h
            put(h, q);
            return;
        }
    }
    // might need frees
    free(q->b);
    free(q->c);
    free(q->min);
    free(q->max);
    free(q->x);
    for(int z = 0; z < q->m + 1; z++)
        free(q->a[z]);
    free(q->a);
    free(q);
}

int branch(node_t* q, double z) {
    if (q->z < z) {
        return 0;
    }
    double min, max; 
    for (int h = 0; h < q->n; ++h) {
        if(!is_integer(&(q->x[h]))){
            if (q->min[h] == -INFINITY) {
                min = 0;
            } else {
                min = q->min[h];
            }
            max = q->max[h];
            if (floor(q->x[h]) < min || ceil(q->x[h]) > max) {
                continue;
            }
            q->h = h;
            q->xh = q->x[h];

            // TODO delete each a, b, c, x, of q // or recycle in other way
            free(q->b);
            free(q->x);
            free(q->c);
            for(int i = 0; i < q->m + 1; ++i){
                free(q->a[i]);
            }
            free(q->a);
            return 1;
        }
    }
    return 0;
}
