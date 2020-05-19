#include "node_set.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bnb.h"
#include "time.h"

int main() {
    // Test init and delete empty node set
    NodeSet* set = initNodeSet(10);
    // add 10 nodes with random z values
    for(int i = 1; i <= 10; i++){
        node_t* x = malloc(sizeof(node_t));
        x->z = rand() % 100;
        put(set, x);
        assert(set->size == i);
    }
    while(!isEmpty(set)){
        node_t* q = get(set);
        // Manually check that all z-values are sorted in descending order
        printf("%lf\n", q->z);
        free(q);
    }

    // Empty the list and see then delete
    deleteNodeSet(set);
    printf("Tests passed\n");
}
