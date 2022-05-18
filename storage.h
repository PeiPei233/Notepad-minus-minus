#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "strlib.h"
#include "global.h"
/**
 * 初始化存储
 */
void initStorage();

/**
 * 获得某一行的字符串
 */ 
string getRowString(int row);

/**
 * 获得总行数
 */ 
int getRowTotal();

/**
 * 获得选中范围的字符串
 */ 
string getSelectString(RCNode startSelect, RCNode endSelect);

/**
 * 获得某一行的长度
 */ 
int getRowLen(int row);

/**
 * 添加字符，并记录操作
 */ 
void addChar(RCNode loc, char ch);

/**
 * 添加字符串，并记录操作
 */ 
void addString(RCNode start, string src);

/**
 * 删除字符，并记录操作
 */ 
void deleteChar(RCNode loc);

/**
 * 删除选中的字符串，并记录操作
 */ 
void deleteSelectString(RCNode start, RCNode endSelect);

#endif