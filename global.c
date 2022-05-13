#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent = (RCNode) {1, 1}; //�������Ͻǵ���������
static RCNode cursor = (RCNode) {1, 1};  //������ڵ���������
static RCNode startSelect = (RCNode) {1, 1}, endSelect = (RCNode) {1, 1};  //ѡ��Χ�����/�յ�����λ��
int bigin, end;

/*
    �û���ȡĿǰ�������Ͻǵ�����λ��
*/
RCNode getWindowCurrentRC() 
{
    return windowCurrent;
}

/*
    �û�����Ŀǰ�������Ͻǵ�����λ��
*/
void setWindowCurrentRC(RCNode newWindowCurrent) 
{
    windowCurrent = newWindowCurrent;
}

/*
    ���ݴ���Ĳ������ù��λ��
*/
void setCursorRC(RCNode newCursor) 
{
    cursor = newCursor;
}

/*
    ��ȡ���λ��
*/
RCNode getCursorRC() 
{
    return cursor;
}

/*
    ��ȡѡ��Χ��ʼλ��
*/
RCNode getSelectStartRC() 
{
    return startSelect;
}

/*
    ��ȡѡ��Χ��ֹλ��
*/
RCNode getSelectEndRC() 
{
    return endSelect;
}

/*
    ����ѡ��Χ��ʼλ��
*/
void setSelectStartRC(RCNode newStartSelect)
{
    startSelect = newStartSelect;
}

/*
    ����ѡ��Χ��ֹλ��
*/
void setSelectEndRC(RCNode newEndSelect)
{
    endSelect = newEndSelect;
}

/*
    �жϹ��λ���Ƿ��ڴ�����
*/
int cursorInWindow(RCNode curCursor) 
{
    
}

/*
    ���Ĺ��λ��ʹ���ܹ�չʾ�ڴ�����
    �������Ѿ��ڴ������򲻵���
*/
void setCursorInWindow() 
{
    
}

/*
    ���ݴ������������������ַ������е�λ�ã��ڼ�����
*/

int numofFormerWords(RCNode curPosition)
{
    char *s = getCurrentString();
    int i = 0, cntl = 0, lens = strlen(s);
    while (i < lens) {
        cntl++;
        if (cntl == curPosition.row) {
            return i + curPosition.column - 1;
        }
        while (i < lens && s[i] != '\n') {
            i++;
        }
        if (s[i] == '\n') i++;
    }
    return lens - 1;
}

static char t[100010];

/**
 * ��õ� row �е�������
 */ 
int getRowLen(int row) {
    char *s = getCurrentString();
    RCNode winCurrent = getWindowCurrentRC();

    int i = 0, j = 0, cntl = 0, lens = strlen(s);
    
    t[0] = t[1] = 0;
    while (i < lens) {
        cntl++;
        t[0] = t[1] = 0;
        while (i < lens && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        if (cntl == row) {
            return j + 1;
        } 
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }
    return 0;
}

/**
 * ����ļ�������
 */ 
int getTotalRow() {
    char *s = getCurrentString();

    int i = 0, j = 0, cntl = 0, lens = strlen(s);
    t[0] = t[1] = 0;
    while (i < lens) {
        cntl++;
        t[0] = t[1] = 0;
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }
    return cntl;
}