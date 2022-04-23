#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//���������ʾ
typedef struct {
    int line;  //��λ��
    int column;  //��λ��
}LCNode;

extern double winHeight, winWidth;	//���ڸ߶ȺͿ��

/*
    ��ȡĿǰ�������Ͻǵ�����λ��
*/
LCNode getWindowCurrentLC();

/*
    ����Ŀǰ�������Ͻǵ�����λ��
*/
void setWindowCurrentLC();

/*
    ���ݴ���Ĳ������ù��λ��
*/
void setCursorLC();

/*
    ��ȡ���λ��
*/
LCNode getCursorLC();

/*
    ��ȡѡ��Χ��ʼλ��
*/
LCNode getSelectStartLC();

/*
    ��ȡѡ��Χ��ֹλ��
*/
LCNode getSelectEndLC();

/*
    ����ѡ��Χ��ʼλ��
*/
void setSelectStartLC();

/*
    ������ʵ��Χ��ֹλ��
*/
void setSelectEndLC();

/*
    �жϹ��λ���Ƿ��ڴ�����
*/
int cursorInWindow();

/*
    ���Ĺ��λ��ʹ���ܹ�չʾ�ڴ�����
    �������Ѿ��ڴ������򲻵���
*/
void setCursorInWindow();

#endif