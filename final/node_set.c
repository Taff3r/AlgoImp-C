#include "node_set.h"
#include <stdlib.h>
#include <stdio.h>
NodeSet* initNodeSet() {
    NodeSet* set = malloc(sizeof(NodeSet));
    set->size = 0;
    set->head = NULL;
    set->tail = NULL;
    return set;
}

int isEmpty(NodeSet* set) {
    return set->size == 0;
}

void deleteSetNode(SetNode* n) {
    // TODO: Add free'ing of e's vars.
    free(n->e);
    free(n);
}

SetNode* initSetNode() {
    SetNode* r = malloc(sizeof(SetNode));
    r->e = NULL;
    r->next = NULL;
    return r;
}

int put(NodeSet* set, node_t* nt) {
    SetNode* s = initSetNode();
    // List is empty
    if (set->head == NULL) {
        set->head = set->tail = s;
    } else {
        // List is non-empty
        set->tail->next = s;
        set->tail = set->tail->next;
    }
    s->e = nt;
    ++(set->size);
    return 1;
}

node_t* get(NodeSet* set) {
    node_t* ret  = set->head->e;
    SetNode* tmp = set->head->next;
    free(set->head);
    set->head = tmp;
    --(set->size);
    return ret;
}

void deleteNodeSet(NodeSet* set) {
    while (set->head != NULL) {
        SetNode* tmp = set->head->next;
        deleteSetNode(set->head);
        --(set->size);
        set->head = tmp;
    }
    free(set);
}
