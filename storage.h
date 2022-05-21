#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "strlib.h"
#include "global.h"

#define BY_CHAR 0x0100
#define BY_STRING 0x0200

/**
 * 初始化存储
 */
void initStorage();

/**
 * 获得某一行的字符串（不要修改！不要修改！不要修改！当作字符串常量来使用！）
 * 如果需要可修改的某一行的字符串请使用getContent()函数！！
 */ 
string getRowContent(int row);

/**
 * 获得总行数
 * 包括文末空行
 */ 
unsigned int getTotalRow();

/**
 * 获得选中范围的字符串的长度
 * 包括所有的'\n'
 * 注意获得的字符串长度范围为[start, end)左闭右开区间
 */
unsigned int getContentLength(RCNode start, RCNode end);

/**
 * 获得选中范围的字符串（为新建字符串，可修改）
 * 用完记得及时free
 * 注意获得的字符串范围为[start, end)左闭右开区间
 * 空字符串则返回NULL
 */ 
string getContent(RCNode start, RCNode end);

/**
 * 获得某一行的长度
 * 如果行末有'\n'则长度中包括'\n'
 */ 
unsigned int getRowLength(int row);

/**
 * 添加字符
 * pos:添加的位置
 * ch:添加的字符
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addContentByChar(RCNode pos, char ch, int doRecord);

/**
 * 添加字符串
 * start:起始位置
 * src:添加的字符串
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addContentByString(RCNode start, string src, int doRecord);

/**
 * 添加内容（略麻烦）
 * by:添加方式 BY_CHAR 通过字符形式添加 BY_STRING 通过字符串形式添加
 * pos:添加位置（起始位置）
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addContent(int by, RCNode pos, char *src, int doRecord);

/**
 * 删除内容
 * start:起始位置
 * end:终止位置
 * doRecord:是否需要记录操作  0-不需要  1-需要
 * 注意删除的范围是[start, end)左闭右开区间
 */ 
void deleteContent(RCNode start, RCNode end, int doRecord);

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
 * 删除字符串
 * start:起始位置
 * end:终止位置
 * doRecord:是否需要记录操作  0-不需要  1-需要
 * 注意删除的范围是[start, end)左闭右开区间
 */ 
void deleteString(RCNode start, RCNode end, int doRecord);

#endif