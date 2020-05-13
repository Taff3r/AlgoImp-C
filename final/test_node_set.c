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

    // Test put node in set and check that size == 1 and set is non-empty
    node_t* n = malloc(sizeof(node_t));
    n->z = 1;
    put(set, n);
    assert(isEmpty(set) == 0);
    assert(set->size == 1);
    
    // put another node and see if size == 2 and set is still non-empty
    node_t* n1 = malloc(sizeof(node_t));
    n1->z = 2;
    put(set, n1);
    assert(set->size == 2);
    assert(isEmpty(set) == 0);

    // Test put duplicate node in set and check that size is still 2
    put(set, n1);
    assert(set->size == 2);
    
    // Test that getting returns the first added value (front of list) and that it got removed
    node_t* r = get(set);
    assert(r == n);
    assert(set->size == 1);
    assert(isEmpty(set) == 0);
    free(r);

    node_t* q = malloc(sizeof(node_t));
    q->z = 1;
    assert(put(set, n1) == 0);
    assert(set->size == 1);
    assert(isEmpty(set) == 0 );
    free(q);

    // check emptying list
    node_t* p = get(set);
    assert(p == n1);
    assert(set->size == 0);
    assert(isEmpty(set) == 1);
    free(p);

    // Check that set was deleted
    deleteNodeSet(set);
    printf("Tests passed\n");
}
