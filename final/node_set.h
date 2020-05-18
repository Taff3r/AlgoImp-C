#ifndef NODE_SET_H
#define NODE_SET_H
typedef struct NodeSet NodeSet;
typedef struct SetNode SetNode;
#include "bnb.h"

struct NodeSet {
    int size;
    int maxNodes;
    SetNode* tail;
    SetNode* head;
};

struct SetNode {
    node_t* e;
    SetNode* next;
};

void deleteSetNode(SetNode*);

NodeSet* initNodeSet(int maxNodes);

SetNode* initSetNode();

void deleteNodeSet(NodeSet*);

int put(NodeSet*, node_t*);

node_t* get(NodeSet*);

int isEmpty(NodeSet*);

#endif
