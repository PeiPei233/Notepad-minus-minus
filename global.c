#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent= (RCNode) {1, 1}; //�������Ͻǵ���������
static RCNode cursor= (RCNode) {1, 1};  //������ڵ���������
static RCNode startSelect =(RCNode) {1, 1}, endSelect= (RCNode) {1, 1};  //ѡ��Χ�����/�յ�����λ��
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
	�Ƚ������ڵ����ַ������е�λ��ǰ�� 
*/
int RCcompare(RCNode start,RCNode end){
	if(start.row>end.row){
		return 1;
	}
	else if(start.row==end.row){
		if(start.column==end.column){
			return 0;
		}
		else if(start.column>end.column)
			return 1;
		else{
			return -1;
		}
	}
	else{
		return -1;
	}
}
