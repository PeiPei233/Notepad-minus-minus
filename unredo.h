#ifndef __UNREDO_H__
#define __UNREDO_H__

#include "global.h"
#include "strlib.h"

#define DO_ADD 0x10
#define DO_DELETE 0x20

typedef struct node {
    int op;
    RCNode loc;
    string str;
    struct node *next;
    struct node *last;
} *linkedList;

/**
 * ¼ÇÂ¼²Ù×÷
 */ 
void record(int op, RCNode loc, string str);

/**
 * ³·Ïú
 */ 
void undo();

/**
 * »Ö¸´
 */ 
void redo();

#endif