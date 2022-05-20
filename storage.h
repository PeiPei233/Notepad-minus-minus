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
unsigned int getTotalRow();

/**
 * 获得选中范围的字符串的长度
 * 注意获得的字符串长度范围为[start, end)左闭右开区间
 */
unsigned int getSelectStringLength(RCNode start, RCNode end);

/**
 * 获得选中范围的字符串
 * 用完记得及时free
 * 注意获得的字符串范围为[start, end)左闭右开区间
 * 空字符串则返回NULL
 */ 
string getSelectString(RCNode start, RCNode end);

/**
 * 获得某一行的长度
 */ 
unsigned int getRowLen(int row);

/**
 * 添加字符
 * pos:添加的位置
 * ch:添加的字符
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addChar(RCNode pos, char ch, int doRecord);

/**
 * 添加字符串，并记录操作
 * start:起始位置
 * src:添加的字符串
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addString(RCNode start, string src, int doRecord);

/**
 * 删除字符
 * pos:删除的位置（注意不是光标前的位置，而是光标所在的位置）
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void deleteChar(RCNode pos, int doRecord);

/**
 * 删除选中的字符串
 * start:起始位置
 * end:终止位置
 * doRecord:是否需要记录操作  0-不需要  1-需要
 * 注意删除的范围是[start, end)左闭右开区间
 */ 
void deleteSelectString(RCNode start, RCNode end, int doRecord);

#endif