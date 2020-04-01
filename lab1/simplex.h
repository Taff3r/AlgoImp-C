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
    double  y;
} simplex_t;

void init_simplex_t(simplex_t* s, int  m, int n){
    s->m   = m;
    s->n   = n;
    // alloc
    s->var = (int*) calloc(sizeof(int) * (n + m), 0); // var are zeroes
    s->b   = (double*) malloc(sizeof(double) * (m));
    s->c   = (double*) malloc(sizeof(double) * (n));
    s->x   = (double*) calloc(sizeof(double) * (n + 1), 0.0); // x are zeroes
    s->a   = (double**) malloc(sizeof(double*) * m);
    for(int i = 0; i != m; ++i)
        s->a[i] = (double*) malloc(sizeof(double) * n);
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
void printSimplex(simplex_t* simp){
    printf("C:\n");
    for(int i = 0; i != simp->n; ++i)
        printf("%lf ", simp->c[i]);
    printf("\nA:");
    for(int i = 0; i != simp->m; ++i){
        printf("\n");
        for(int k = 0; k != simp->n; ++k)
            printf("%lf ", simp->a[i][k]);
    }
    printf("\nB:\n");
    for(int i = 0; i != simp->m; ++i)
        printf("%lf ", simp->b[i]);
    printf("\n");
}

void printConstraints(simplex_t* simp) {
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
