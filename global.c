#include "global.h"
#include "libgraphics.h"

static LCNode windowCurrent; //�������Ͻǵ���������
static LCNode cursor;  //������ڵ���������
static LCNode startSelect, endSelect;  //ѡ��Χ�����/�յ�����λ��

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
bool cursorInWindow();

/*
    ���Ĺ��λ��ʹ���ܹ�չʾ�ڴ�����
    �������Ѿ��ڴ������򲻵���
*/
void setCursorInWindow();