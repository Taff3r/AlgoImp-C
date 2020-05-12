#ifndef BNB_H
#define BNB_H

typedef struct node_t node_t;

struct node_t {
    int m;
    int n;
    int k;
    int h;
    double xh;
    double ak;
    double bk;
    double* min;
    double* max;
    double** a;
    double* b;
    double* x;
    double* c;
    double z;
};

node_t* initial_node(int m, int n, double** a, double* b, double* c);

node_t* extend(node_t* p, int m, int n, double** a, double* b, double* c, int k, double ak, double bk);

int is_integer(double* xp);

int integer(node_t* p);

void bound(node_t* p, int h, double* zp, double* x);

int branch(node_t* q, double z);

void succ(node_t* p, int h, int m, int n, double** a, double* b, double* c, int k, double ak, double bk, double* zp, double* x);

double intopt(int m, int n, double** a, double* b, double* c, double* x);
#endif
