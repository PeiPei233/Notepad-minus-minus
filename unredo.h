#ifndef __UNREDO_H__
#define __UNREDO_H__

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
 * ��¼����
 * op ��������: OP_ADD ����ַ��� OP_DELETE ɾ���ַ���
 * pos ����λ��
 * str ���/ɾ�����ַ���
 */ 
void record(int op, RCNode pos, string str);

/**
 * ʵ�ֳ�������
 */ 
void undo();

/**
 * ʵ����������
 */ 
void redo();

/**
 * ���㴫���ַ��Ľ�������
 * ���뿪ʼ���������м���ַ���
 * */
RCNode endPos(RCNode startPos, string str);

#endif