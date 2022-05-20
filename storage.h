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
unsigned int getTotalRow();

/**
 * ���ѡ�з�Χ���ַ����ĳ���
 * ע���õ��ַ������ȷ�ΧΪ[start, end)����ҿ�����
 */
unsigned int getSelectStringLength(RCNode start, RCNode end);

/**
 * ���ѡ�з�Χ���ַ���
 * ����ǵü�ʱfree
 * ע���õ��ַ�����ΧΪ[start, end)����ҿ�����
 * ���ַ����򷵻�NULL
 */ 
string getSelectString(RCNode start, RCNode end);

/**
 * ���ĳһ�еĳ���
 */ 
unsigned int getRowLen(int row);

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
 * pos:ɾ����λ�ã�ע�ⲻ�ǹ��ǰ��λ�ã����ǹ�����ڵ�λ�ã�
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void deleteChar(RCNode pos, int doRecord);

/**
 * ɾ��ѡ�е��ַ���
 * start:��ʼλ��
 * end:��ֹλ��
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 * ע��ɾ���ķ�Χ��[start, end)����ҿ�����
 */ 
void deleteSelectString(RCNode start, RCNode end, int doRecord);

#endif