#include <stdio.h>
int main(){
    // 1
    printf("hello world!\n");
    FILE* fp;
    fp = fopen("constraints", "r");
    int m, n;
    fscanf(fp, "%d %d", &m,&n);
    int c[n];
    int a[m][n];
    int b[m][n];
    printf("m = %d, n = %d\n", m,n);
    for(int i = 0; i != n; ++i)
        fscanf(fp, "%d", &(c[i]));
    for(int i = 0; i != n; ++i)
        fscanf(fp, "%d", &(a[0][i]));

    printf("c\n");
    for(int i = 0; i != n; ++i){
        printf("%d ", c[i]);
    }
    fclose(fp);
}
