/*
    ��Ҫʵ���ļ���������ع���
*/
#ifndef __FILE_H__
#define __FILE_H__

/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
*/
void openFile();

/*
    ��ʼ�������ļ�
*/
void initCache();

/*
    ��ȡ��ǰ����ʾ���ַ���
*/
char *getCurrentString();

/*
    �Զ��崫��Ĳ��� �細�����Ͻǵ��������� ���ݴ�������ӻ����ļ��и���currentString
*/
void updateCurrentString();

/*
    ���ݴ�����ַ�����currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void addChar();

/*
    ���ݴ�����ַ�������currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void addString();

/*
    ���ݵ�ǰ���λ�ã�ɾ�����ǰ��һ���ַ�����currentString�뻺���ļ��У�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteChar();

/*
    ���ݵ�ǰѡ�еķ�Χ��ɾ��ѡ�е��ַ�������currentString��cacheFile�У�
    �����¹���Լ�ѡ�з�Χ���������꣨���б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteSelectString();

#endif