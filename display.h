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

void display();

/*
    ���˵� �����ݲ˵�ѡ��ִ�ж�Ӧ�ĺ���������
    ��û�д��κ��ļ�ʱ��ʾ�½��ļ�
*/
void drawMenu();

/*
    �ڷ�Χ�ڻ��ı�������Ĳ����������Ͻ�����(x, y) ���εĳ���(w, h) �и� �Ƿ���Ҫ�Զ����� �ȵ�
*/
void drawTextArea();

/*
    ���Ʋ��Ҵ��� ����һ�����������ı���Ͳ��Ұ�ť��
    ����������ļ��ı������Ͻ�
    �Ȼ��Ƶײ��ı� �ڻ��Ʋ��Ҵ���ʱ�Ȼ���һ����ɫ���θ��ǵ��µ��ı����ڻ��Ʋ��Ҵ���
    �����滻����ͬ��
*/
void drawFindArea();

/*
    �����滻���� ���������ı��򣨷ֱ��ǲ��Һ��滻���Լ��滻��ť��
*/
void drawReplaceArea();

#endif