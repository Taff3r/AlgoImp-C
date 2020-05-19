#include "node_set.h"
#include <stdlib.h>
#include <stdio.h>
NodeSet* initNodeSet(int maxNodes) {
    NodeSet* set = malloc(sizeof(NodeSet));
    set->size = 0;
    set->maxNodes = maxNodes;
    set->head = NULL;
    set->tail = NULL;
    return set;
}

int isEmpty(NodeSet* set) {
    return set->size == 0;
}

void deleteSetNode(SetNode* n) {
    free(n);
}

SetNode* initSetNode() {
    SetNode* r = malloc(sizeof(SetNode));
    r->e = NULL;
    r->next = NULL;
    return r;
}

void printOrder(NodeSet* s) {
    SetNode* n = s->head;
    while(n != NULL) {
        printf("%lf \n", n->e->z);
        n = n->next;
    }
}

void putBack(NodeSet* set, node_t* nt) {
    if(set->size >= set->maxNodes) {
        return;
    }

    SetNode* i = initSetNode();
    i->e = nt;
    set->tail->next = i;
    set->tail = i;
    ++(set->size);
}

void put(NodeSet* set, node_t* nt) {
    // Fuck off were full
    if(set->size >= set->maxNodes) {
        return;
    }
    // List is empty
    if (set->head == NULL) {
        SetNode* s = initSetNode();
        set->head = set->tail = s;
        s->e = nt;
        ++(set->size);
    } else {
        // List is non-empty
        // Check if the heads element->z has lower value than the one were trying to insert.
        // If it is we can insert it much faster.
        if(set->head->e->z <= nt->z){
            SetNode* s = initSetNode();
            SetNode* tmp = set->head;
            set->head = s;
            s->e = nt;
            s->next = tmp;
            ++(set->size);
        } else if(nt->z <= set->tail->e->z) { // Check if it should be inserted at the tail of the list, also faster
            SetNode* s = initSetNode();
            SetNode* tmp = set->tail;
            set->tail = s;
            tmp->next = s;
            s->e = nt;
            ++(set->size);
        } else {
            // Find the correct place to insert the node.
            // List is atleast size of 2 before it gets here
            //
                SetNode* curr = set->head;
                SetNode* next = set->head->next; 
                while(1){
                    if (next->e->z <= nt->z ){
                        SetNode* i = initSetNode();
                        i->e = nt;
                        curr->next = i;
                        i->next = next;
                        ++(set->size);
                        return;
                    } else {
                        curr = next;
                        next = next->next;
                    }
                }
        }
    }
}

node_t* get(NodeSet* set) {
    node_t* ret  = set->head->e;
    SetNode* tmp = set->head->next;
    deleteSetNode(set->head);
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
