#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "strlib.h"
#include "global.h"
/**
 * ��ʼ���洢
 */
void initStorage();

/**
 * ���ĳһ�е��ַ���
 */ 
string getRowString(int row);

/**
 * ���������
 */ 
int getRowTotal();

/**
 * ���ѡ�з�Χ���ַ���
 */ 
string getSelectString(RCNode startSelect, RCNode endSelect);

/**
 * ���ĳһ�еĳ���
 */ 
int getRowLen(int row);

/**
 * ����ַ�
 * pos:��ӵ�λ��
 * ch:��ӵ��ַ�
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addChar(RCNode pos, char ch, int doRecord);

/**
 * ����ַ���������¼����
 * start:��ʼλ��
 * src:��ӵ��ַ���
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addString(RCNode start, string src, int doRecord);

/**
 * ɾ���ַ�
 * pos:��ӵ�λ��
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void deleteChar(RCNode pos, int doRecord);

/**
 * ɾ��ѡ�е��ַ���
 * start:��ʼλ��
 * end:��ֹλ��
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void deleteSelectString(RCNode start, RCNode end, int doRecord);

#endif