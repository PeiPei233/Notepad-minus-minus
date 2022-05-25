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
 * ��ȡ�Ҽ��˵���ʾ״̬
 */ 
int getContextMenuDisplayState();

/**
 * ���Ĳ��Ҵ�����ʾ״̬
 */ 
void setFindDisplayState(int newFindDisplayState);

/**
 * �����滻������ʾ״̬
 */ 
void setReplaceDisplayState(int newReplaceDisplayState);

/**
 * �����Ҽ��˵���ʾ״̬
 */ 
void setContextMenuDisplayState(int newContextMenuDisplayState);

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
 * �����Ҽ��˵������Ͻ�λ��
 */ 
void setContextMenuXY(double x, double y);

/**
 * ��ʾ��������
 */ 
void display();

/**
 * �����ڵ�(x, y)��������ת��Ϊ��������
 */ 
RCNode XYtoRC(int x, int y);

/**
 * ���ڴ�����̰���ʱ�Ŀ�ݼ�
 * key      ���µİ���
 * isShift  �Ƿ���Shift
 * isCtrl   �Ƿ���Ctrl
 * isTyping �Ƿ�������״̬
 */ 
void processShortcutKey(int key, int isShift, int isCtrl, int isTyping);

/**
 * ����겻�ڴ����У����ƶ�����ʹ����ڴ�����
 */ 
void setCursorInWindow();

#endif