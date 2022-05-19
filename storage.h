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
 * 添加字符
 * pos:添加的位置
 * ch:添加的字符
 * record:是否需要记录操作  0-不需要  1-需要
 */ 
void addChar(RCNode pos, char ch, int record);

/**
 * 添加字符串，并记录操作
 * start:起始位置
 * src:添加的字符串
 * record:是否需要记录操作  0-不需要  1-需要
 */ 
void addString(RCNode start, string src, int record);

/**
 * 删除字符
 * pos:添加的位置
 * record:是否需要记录操作  0-不需要  1-需要
 */ 
void deleteChar(RCNode pos, int record);

/**
 * 删除选中的字符串
 * start:起始位置
 * end:终止位置
 * record:是否需要记录操作  0-不需要  1-需要
 */ 
void deleteSelectString(RCNode start, RCNode end, int record);

#endif