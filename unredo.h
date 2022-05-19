#ifndef __UNREDO_H__
#define __UNREDO_H__

#include "global.h"
#include "strlib.h"

#define OP_ADD 0x10
#define OP_DELETE 0x20

typedef struct node {
    int op;
    RCNode pos;
    string str;
    struct node *next;
    struct node *last;
} *linkedList;

/**
 * ¼ÇÂ¼²Ù×÷
 * op ²Ù×÷ÀàÐÍ: OP_ADD Ìí¼Ó×Ö·û´® OP_DELETE É¾³ý×Ö·û´®
 * pos ²Ù×÷Î»ÖÃ
 * str Ìí¼Ó/É¾³ýµÄ×Ö·û´®
 */ 
void record(int op, RCNode pos, string str);

/**
 * ³·Ïú
 */ 
void undo();

/**
 * »Ö¸´
 */ 
void redo();

#endif