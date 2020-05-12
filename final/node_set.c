#include "node_set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

NodeSet* initNodeSet() {
    NodeSet* set = malloc(sizeof(NodeSet));
    set->size = 0;
    set->head = 0;
    set->tail = 0;
    return set;
}

int isEmpty(NodeSet* set) {
    return set->size == 0;
}

void deleteSetNode(SetNode* n) {
    free(n->e);
    free(n);
}
SetNode* initSetNode() {
    SetNode* r = malloc(sizeof(SetNode));
    r->e = NULL;
    r->next = NULL;
    return r;
}
// TODO Check for duplicates
// Use Floyds Turtle & Hare to find dups?
void put(NodeSet* set, node_t* nt) {
    SetNode* s = initSetNode();
    // List is empty
    if (set->head == NULL) {
        set->head = set->tail = s;
    } else {
        // List is non-empty
        SetNode* tmp = set->head;
        while(tmp != NULL) {
            // Duplicate found
            if(tmp->e == nt){
                // Delete s and return
                free(s);
                return;
            }
            // Move forward in list
            tmp = tmp->next;
        }
        set->tail->next = s;
        set->tail = set->tail->next;
    }
    s->e = nt;
    ++(set->size);
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
        set->head = tmp;
    }
    free(set);
}

int main() {
    NodeSet* set = initNodeSet();
    assert(isEmpty(set) == 1);
    printf("%ld\n", sizeof(node_t));
    node_t* n = malloc(sizeof(node_t));
    put(set, n);
    assert(isEmpty(set) == 0);
    assert(isEmpty(set) != 1);
    assert(set->size == 1);
    node_t* n1 = malloc(sizeof(node_t));
    put(set, n1);
    assert(set->size == 2);
    assert(isEmpty(set) == 0);
    put(set, n1);
    assert(set->size == 2);
    printf("Tests passed\n");
    deleteNodeSet(set);
}
