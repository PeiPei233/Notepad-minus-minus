#ifndef __RECORD_H__
#define __RECORD_H__

#include "global.h"
#include "strlib.h"
#include "storage.h"
#include <string.h>

#define OP_ADD 0x10
#define OP_DELETE 0x20

typedef struct node {
    int op;
    RCNode pos;
    string str;
    struct node *next;
    struct node *last;
} linkedList;

/**
 * 初始化操作记录
 * 若之前有记录过操作数据，则要全部清除！！
 */ 
void initRecord();

/**
 * 记录操作
 * op 操作类型: OP_ADD 添加字符串 OP_DELETE 删除字符串
 * pos 操作位置
 * str 添加/删除的字符串
 */ 
void record(int op, RCNode pos, string str);

/**
 * 实现撤销操作
 */ 
void undo();

/**
 * 实现重做操作
 */ 
void redo();

/**
 * 计算传入字符的结束坐标
 * 传入开始的坐标与中间的字符串
 * */
RCNode endPos(RCNode startPos, string str);

#endif