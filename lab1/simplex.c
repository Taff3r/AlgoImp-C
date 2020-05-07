#include "simplex.h"
#include <stdlib.h>
void init_simplex_t(simplex_t* s, int  m, int n){
    s->m   = m;
    s->n   = n;
    // alloc
    s->var = (int*) calloc((n + m + 1), sizeof(int)); // var are zeroes
    s->b   = (double*) calloc(m, sizeof(double));
    s->c   = (double*) calloc(n, sizeof(double)); 
    s->x   = (double*) calloc((n + 1), sizeof(double)); // x are zeroes
    s->a   = (double**) calloc(m , sizeof(double*));
    for(int i = 0; i != m; ++i){
        s->a[i] = (double*) calloc(n + 1, sizeof(double));
    }

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

void delete_simp(simplex_t* s){
    free(s->var);
    free(s->b);
    free(s->x);
    free(s->c);
    for (int i = 0; i != s->m; ++i)
        free(s->a[i]); // Free all sub pointers
    free(s->a); // Free other pointer
    free(s);
}


