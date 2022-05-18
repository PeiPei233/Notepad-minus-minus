/**
 * 
 * ���ļ����ڴ����ַ�������Ĵ������
 * 
 */ 

#include "storage.h"
#include "unredo.h"

int sizeR;      //�з�����������
int capR;       //�з���ɳ�������
int *sizeL;     //�з�����������
int *capL;      //�з���ɳ�������
char **str;

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
 * ����ַ�������¼����
 */ 
void addChar(RCNode loc, char ch);

/**
 * ����ַ���������¼����
 */ 
void addString(RCNode start, string src);

/**
 * ɾ���ַ�������¼����
 */ 
void deleteChar(RCNode loc);

/**
 * ɾ��ѡ�е��ַ���������¼����
 */ 
void deleteSelectString(RCNode start, RCNode endSelect);