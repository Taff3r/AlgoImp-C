#include <stdio.h>
#include "simplex.h"
int main(){
    // 1
    simplex_t* simp = malloc(sizeof(simplex_t));
    printf("hello world!\n");
    FILE* fp;
    fp = fopen("constraints", "r");
    int m,n;
    fscanf(fp, "%d %d", &m, &n);
    int a[] = {1,2,3,4};
    init_simplex_t(simp, m, n, a);
    printf("%d %d", simp->m, simp->n);
    for (int k = 0; k != m + n; ++k){
        printf(" %d", simp->var[k]);
    }
    fclose(fp);
}
