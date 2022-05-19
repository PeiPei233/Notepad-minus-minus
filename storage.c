/**
 * 
 * 该文件用于处理字符串数组的储存操作
 * 
 */ 

#include "storage.h"
#include "unredo.h"

int sizeR;      //列方向所用总数
int capR;       //列方向可承载总数
int *sizeL;     //行方向所用总数
int *capL;      //行方向可承载总数
char **str;

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