#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "strlib.h"
#include "global.h"

#define BY_CHAR 0x0100
#define BY_STRING 0x0200

/**
 * ��ʼ���洢
 * ��֮ǰ�����ݻᱻ�����֮ǰ�����ݻᶪʧ��
 */
void initStorage();

/**
 * ���ĳһ�е��ַ�������Ҫ�޸ģ���Ҫ�޸ģ���Ҫ�޸ģ������ַ���������ʹ�ã���
 * �����Ҫ���޸ĵ�ĳһ�е��ַ�����ʹ��getContent()��������
 */ 
const string getRowContent(int row);

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
 * recordID:�����Ķ�Ӧ ID   �ر�أ��������� recordID Ϊ 0 �򲻼�¼����
 */ 
void addContentByChar(RCNode pos, char ch, int recordID);

/**
 * ����ַ���
 * start:��ʼλ��
 * src:��ӵ��ַ���
 * recordID:�����Ķ�Ӧ ID   �ر�أ��������� recordID Ϊ 0 �򲻼�¼����
 */ 
void addContentByString(RCNode start, string src, int recordID);

/**
 * ������ݣ����鷳��
 * by:��ӷ�ʽ BY_CHAR ͨ���ַ���ʽ��� BY_STRING ͨ���ַ�����ʽ���
 * pos:���λ�ã���ʼλ�ã�
 * recordID:�����Ķ�Ӧ ID   �ر�أ��������� recordID Ϊ 0 �򲻼�¼����
 */ 
void addContent(int by, RCNode pos, char *src, int recordID);

/**
 * ɾ������
 * start:��ʼλ��
 * end:��ֹλ��
 * recordID:�����Ķ�Ӧ ID   �ر�أ��������� recordID Ϊ 0 �򲻼�¼����
 * ע��ɾ���ķ�Χ��[start, end)����ҿ�����
 */ 
void deleteContent(RCNode start, RCNode end, int recordID);

#endif