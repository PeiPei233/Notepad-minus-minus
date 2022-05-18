#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "strlib.h"
#include "global.h"

string getRowString();
int getRowTotal();
string getSelectString(RCNode startSelect, RCNode endSelect);
int getRowLen(int row);
void addChar(RCNode loc);
void addString(RCNode start, string src);
void deleteChar(RCNode loc);
void deleteSelectString(RCNode start, RCNode endSelect);

#endif