/*
    ��Ҫʵ���ļ���������ع���
*/

#include <stdio.h>
#include "file.h"

#define _N 1000010

static char currentString[_N];
static FILE *currentFile;  //��ǰ�򿪵��ļ�
static FILE *cacheFile;    //�����ļ�
static int isSaved;     //�Ƿ񱣴�

/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
*/
void openFile() {

}

/*
    ��ʼ�������ļ�
*/
void initCache() {

}

/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
*/
void saveFile() {

}

/*
    ��ȡ��ǰ�ı���״̬
    �Ƿ񱣴�
*/
int getSaveState() {

}

/*
    ��ȡ��ǰ����ʾ���ַ���
*/
char *getCurrentString() {

}

/*
    �Զ��崫��Ĳ��� �細�����Ͻǵ��������� ���ݴ�������ӻ����ļ��и���currentString
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    ���ݴ�����ַ�����currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void addChar(char ch) {
    RCNode cursor = getCursorRC();
}

/*
    ���ݴ�����ַ�������currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void addString(char *src) {
    RCNode cursor = getCursorRC();
}

/*
    ���ݵ�ǰ���λ�ã�ɾ�����ǰ��һ���ַ�����currentString�뻺���ļ��У�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteChar() {
    RCNode cursor = getCursorRC();
}

/*
    ���ݵ�ǰѡ�еķ�Χ��ɾ��ѡ�е��ַ�������currentString��cacheFile�У�
    �����¹���Լ�ѡ�з�Χ���������꣨���б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteSelectString() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
}