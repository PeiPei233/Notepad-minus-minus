/*
    ��Ҫ�����ı��Ļ��� ������ �����滻���ڵĻ��� ��ʽ����ѡ��Ļ���
*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "global.h"

/*
    ���˵� �����ݲ˵�ѡ��ִ�ж�Ӧ�ĺ���������
    ��û�д��κ��ļ�ʱ��ʾ�½��ļ�
*/
void drawMenu();

/*
    �ڷ�Χ�ڻ��ı�������Ĳ����������Ͻ�����(x, y) ���εĳ���(w, h) �и� �Ƿ���Ҫ�Զ����� �ȵ�
*/
void drawTextWithinArea(double x, double y, double w, double h);

/*
    ���ı��򴰿��е�����λ��(r, c)��ʾ���
    ��Ҫ�Ĳ������������������ı������Ͻ����꣬�иߵȵ�
*/
void drawCursor(double rowHeight);

/*
    ����ѡ�еķ�Χ���Ƹ����ı���ɫ
    ע��һ��Ҫ�Ȼ����ı���ɫ �ٻ����ı�
    ������������������ѡ�з�Χ����ʼ������������ֹ��������
*/
void drawSelectRange(double rowHeight);

/*
    ���Ʋ��Ҵ��� ����һ�����������ı���Ͳ��Ұ�ť��
    ����������ļ��ı������Ͻ�
    �Ȼ��Ƶײ��ı� �ڻ��Ʋ��Ҵ���ʱ�Ȼ���һ����ɫ���θ��ǵ��µ��ı����ڻ��Ʋ��Ҵ���
    �����滻����ͬ��
*/
void drawFindArea(double x, double y, double w, double h);

/*
    �����滻���� ���������ı��򣨷ֱ��ǲ��Һ��滻���Լ��滻��ť��
*/
void drawReplaceArea(double x, double y, double w, double h);

#endif