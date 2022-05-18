#ifndef __UNREDO_H__
#define __UNREDO_H__

#include "global.h"

enum {
    DO_ADD;
    DO_DELETE;
};

// typedef struct node {
//     int op;
//     RCnode loc;
//     string str;
//     struct node *next;
//     struct node *last;
// } *linkedList;

void record(int op, RCNode loc, string str);
void undo();
void redo();

#endif