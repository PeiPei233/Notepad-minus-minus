#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent; //�������Ͻǵ���������
static RCNode cursor;  //������ڵ���������
static RCNode startSelect, endSelect;  //ѡ��Χ�����/�յ�����λ��

/*
    ��ȡĿǰ�������Ͻǵ�����λ��
*/
RCNode getWindowCurrentRC() {

}

/*
    ����Ŀǰ�������Ͻǵ�����λ��
*/
void setWindowCurrentRC(RCNode newWindowCurrent) {

}

/*
    ���ݴ���Ĳ������ù��λ��
*/
void setCursorRC(RCNode newCursor) {

}

/*
    ��ȡ���λ��
*/
RCNode getCursorRC() {

}

/*
    ��ȡѡ��Χ��ʼλ��
*/
RCNode getSelectStartRC() {

}

/*
    ��ȡѡ��Χ��ֹλ��
*/
RCNode getSelectEndRC() {

}

/*
    ����ѡ��Χ��ʼλ��
*/
void setSelectStartRC(RCNode newStartSelect) {

}

/*
    ������ʵ��Χ��ֹλ��
*/
void setSelectEndRC(RCNode newEndSelect) {

}

/*
    �жϹ��λ���Ƿ��ڴ�����
*/
int cursorInWindow() {

    //return 0(false) or 1(true)
}

/*
    ���Ĺ��λ��ʹ���ܹ�չʾ�ڴ�����
    �������Ѿ��ڴ������򲻵���
*/
void setCursorInWindow() {

}