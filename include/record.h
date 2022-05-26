#ifndef __RECORD_H__
#define __RECORD_H__

#include "global.h"
#include "strlib.h"
#include "storage.h"
#include <string.h>

#define OP_ADD 0x10
#define OP_DELETE 0x20

typedef struct node {
    int op;             //����
    int id;             //���β�����Ӧ�� id     ��������Ĳ�����Ӧͬһ�� id ����Ϊͬһ������
    RCNode pos;         //����λ��
    string str;         //�仯���ַ���
    struct node *next;  //��һ���ڵ�
    struct node *last;  //��һ���ڵ�
} linkedList;

/**
 * ��ʼ��������¼
 * ��֮ǰ�м�¼���������ݣ���Ҫȫ���������
 */ 
void initRecord();

/**
 * ����µ�record ID
 */ 
int newRecordID();

/**
 * ��¼����
 * op ��������: OP_ADD ����ַ��� OP_DELETE ɾ���ַ���
 * pos ����λ��
 * str ���/ɾ�����ַ���
 */ 
void record(int op, RCNode pos, const string str, int recordID);

/**
 * ʵ�ֳ�������
 */ 
void undo();

/**
 * ʵ����������
 */ 
void redo();

#endif