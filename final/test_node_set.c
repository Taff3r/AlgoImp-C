#include "node_set.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "bnb.h"

int main() {
    // Test init and delete empty node set
    NodeSet* set = initNodeSet();
    deleteNodeSet(set);

    set = initNodeSet();
    // Test if set starts empty
    assert(isEmpty(set) == 1);
    node_t* n1 = malloc(sizeof(node_t));
    put(set, n1);
    assert(set->size == 1);
    assert(isEmpty(set) == 0);

    node_t* n2 = malloc(sizeof(node_t));
    put(set, n2);
    assert(set->size == 2);
    assert(isEmpty(set) == 0);

    node_t* r = get(set);
    assert(r == n1);
    assert(set->size == 1);
    assert(isEmpty(set) == 0);
    free(r);
  
    // add 10 nodes
    for(int i = 1; i <= 10; i++){
        node_t* x = malloc(sizeof(node_t));
        put(set, x);
        assert(set->size == 1 + i);
    }


    // Empty the list and see then delete
    while(isEmpty(set)){
        free(get(set));
    }
    deleteNodeSet(set);
    printf("Tests passed\n");
}
