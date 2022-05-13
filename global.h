#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "callback.h"

#define MAX 100000

//���������ʾ
typedef struct {
    int row;  //��λ��
    int column;  //��λ��
}RCNode;

extern double winHeight, winWidth;	//���ڸ߶ȺͿ��

/*
    ��ȡĿǰ�������Ͻǵ�����λ��
*/
RCNode getWindowCurrentRC();

/*
    ����Ŀǰ�������Ͻǵ�����λ��
*/
void setWindowCurrentRC(RCNode newWindowCurrent);

/*
    ���ݴ���Ĳ������ù��λ��
*/
void setCursorRC(RCNode newCursor);

/*
    ��ȡ���λ��
*/
RCNode getCursorRC();

/*
    ��ȡѡ��Χ��ʼλ��
*/
RCNode getSelectStartRC();

/*
    ��ȡѡ��Χ��ֹλ��
*/
RCNode getSelectEndRC();

/*
    ����ѡ��Χ��ʼλ��
*/
void setSelectStartRC(RCNode newStartSelect);

/*
    ����ѡ��Χ��ֹλ��
*/
void setSelectEndRC(RCNode newEndSelect);

/*
    �жϹ��λ���Ƿ��ڴ�����
*/
int cursorInWindow(RCNode curCursor);

/*
    ���Ĺ��λ��ʹ���ܹ�չʾ�ڴ�����
    �������Ѿ��ڴ������򲻵���
*/
void setCursorInWindow();

/*
    ʵ�ֽ������������ת��Ϊ�ܵ��ַ������������ļ����еĶ�ȡ
*/
int  numofFormerWords(RCNode curPosition);

/**
 * ��õ� row �е�������
 */ 
int getRowLen(int row);

/**
 * ����ļ�������
 */ 
int getTotalRow();

#endif