/*
    ��Ҫ�����ı��Ļ��� ������ �����滻���ڵĻ��� ��ʽ����ѡ��Ļ���
*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "global.h"

/**
 * ��ȡ���Ҵ�����ʾ״̬
 */ 
int getFindDisplayState();

/**
 * ��ȡ�滻������ʾ״̬
 */ 
int getReplaceDisplayState();

/**
 * ���Ĳ��Ҵ�����ʾ״̬
 */ 
void setFindDisplayState(int newFindDisplayState);

/**
 * �����滻������ʾ״̬
 */ 
void setReplaceDisplayState(int newReplaceDisplayState);

/**
 * ��ȡ�ı�������ʾ״̬
 */
int getTextDisplayState();

/**
 * ��ȡ��ǰ��ʾ��������
 */ 
int getTotalDisplayRow();

/**
 * ���µ�ǰ��ʾ��������
 */ 
void updateTotalDisplayRow();

/**
 * ��ʾ��������
 */ 
void display();

/**
 * �����ڵ�(x, y)��������ת��Ϊ��������
 */ 
RCNode XYtoRC(int x, int y);

#endif