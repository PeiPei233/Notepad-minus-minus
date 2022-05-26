#ifndef __RECORD_H__
#define __RECORD_H__

#include "global.h"
#include "strlib.h"
#include "storage.h"
#include <string.h>

#define OP_ADD 0x10
#define OP_DELETE 0x20

typedef struct node {
    int op;             //操作
    int id;             //本次操作对应的 id     如果连续的操作对应同一个 id 则视为同一个操作
    RCNode pos;         //操作位置
    string str;         //变化的字符串
    struct node *next;  //下一个节点
    struct node *last;  //上一个节点
} linkedList;

/**
 * 初始化操作记录
 * 若之前有记录过操作数据，则要全部清除！！
 */ 
void initRecord();

/**
 * 获得新的record ID
 */ 
int newRecordID();

/**
 * 记录操作
 * op 操作类型: OP_ADD 添加字符串 OP_DELETE 删除字符串
 * pos 操作位置
 * str 添加/删除的字符串
 */ 
void record(int op, RCNode pos, const string str, int recordID);

/**
 * 实现撤销操作
 */ 
void undo();

/**
 * 实现重做操作
 */ 
void redo();

#endif