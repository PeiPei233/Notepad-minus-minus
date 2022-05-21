#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "strlib.h"
#include "global.h"

#define BY_CHAR 0x0100
#define BY_STRING 0x0200

/**
 * ��ʼ���洢
 */
void initStorage();

/**
 * ���ĳһ�е��ַ�������Ҫ�޸ģ���Ҫ�޸ģ���Ҫ�޸ģ������ַ���������ʹ�ã���
 * �����Ҫ���޸ĵ�ĳһ�е��ַ�����ʹ��getContent()��������
 */ 
string getRowContent(int row);

/**
 * ���������
 * ������ĩ����
 */ 
unsigned int getTotalRow();

/**
 * ���ѡ�з�Χ���ַ����ĳ���
 * �������е�'\n'
 * ע���õ��ַ������ȷ�ΧΪ[start, end)����ҿ�����
 */
unsigned int getContentLength(RCNode start, RCNode end);

/**
 * ���ѡ�з�Χ���ַ�����Ϊ�½��ַ��������޸ģ�
 * ����ǵü�ʱfree
 * ע���õ��ַ�����ΧΪ[start, end)����ҿ�����
 * ���ַ����򷵻�NULL
 */ 
string getContent(RCNode start, RCNode end);

/**
 * ���ĳһ�еĳ���
 * �����ĩ��'\n'�򳤶��а���'\n'
 */ 
unsigned int getRowLength(int row);

/**
 * ����ַ�
 * pos:��ӵ�λ��
 * ch:��ӵ��ַ�
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addContentByChar(RCNode pos, char ch, int doRecord);

/**
 * ����ַ���
 * start:��ʼλ��
 * src:��ӵ��ַ���
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addContentByString(RCNode start, string src, int doRecord);

/**
 * ������ݣ����鷳��
 * by:��ӷ�ʽ BY_CHAR ͨ���ַ���ʽ��� BY_STRING ͨ���ַ�����ʽ���
 * pos:���λ�ã���ʼλ�ã�
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addContent(int by, RCNode pos, char *src, int doRecord);

/**
 * ɾ������
 * start:��ʼλ��
 * end:��ֹλ��
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 * ע��ɾ���ķ�Χ��[start, end)����ҿ�����
 */ 
void deleteContent(RCNode start, RCNode end, int doRecord);

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
 * ɾ���ַ���
 * start:��ʼλ��
 * end:��ֹλ��
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 * ע��ɾ���ķ�Χ��[start, end)����ҿ�����
 */ 
void deleteString(RCNode start, RCNode end, int doRecord);

#endif