#include <stdio.h>
#include "simplex.h"
int main(){
    // 1
    simplex_t* simp = malloc(sizeof(simplex_t));
    printf("hello world!\n");
    FILE* fp;
    fp = fopen("constraints", "r");
    // Scan m && n
    int m,n;
    fscanf(fp, "%d %d", &m, &n);
    init_simplex_t(simp, m, n);
    // Scan c
    for (int i = 0; i != n; ++i)
        fscanf(fp, "%lf", &simp->c[i]);

    // Scan a
    for (int i = 0; i != m; ++i)
        for (int k = 0; k != n; ++k)
            fscanf(fp, "%lf", &simp->a[i][k]);
    // Scan b
    for (int i = 0; i != n; ++i)
        fscanf(fp, "%lf", &simp->b[i]);

    printSimplex(simp);
    fclose(fp);
    delete_simp(simp);
}


        
