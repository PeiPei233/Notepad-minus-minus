#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent= (RCNode) {1, 1}; //窗口左上角的行列坐标
static RCNode cursor= (RCNode) {1, 1};  //光标所在的行列坐标
static RCNode startSelect =(RCNode) {1, 1}, endSelect= (RCNode) {1, 1};  //选择范围的起点/终点行列位置
int bigin, end;

/*
    用户获取目前窗口左上角的行列位置
*/
RCNode getWindowCurrentRC() 
{
    return windowCurrent;
}

/*
    用户更改目前窗口左上角的行列位置
*/
void setWindowCurrentRC(RCNode newWindowCurrent) 
{
    windowCurrent = newWindowCurrent;
}

/*
    根据传入的参数设置光标位置
*/
void setCursorRC(RCNode newCursor) 
{
    cursor = newCursor;
}

/*
    获取光标位置
*/
RCNode getCursorRC() 
{
    return cursor;
}

/*
    获取选择范围起始位置
*/
RCNode getSelectStartRC() 
{
    return startSelect;
}

/*
    获取选择范围终止位置
*/
RCNode getSelectEndRC() 
{
    return endSelect;
}

/*
    设置选择范围起始位置
*/
void setSelectStartRC(RCNode newStartSelect)
{
    startSelect = newStartSelect;
}

/*
    设置选择范围终止位置
*/
void setSelectEndRC(RCNode newEndSelect)
{
    endSelect = newEndSelect;
}
/*
	比较两个节点在字符串流中的位置前后 
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
