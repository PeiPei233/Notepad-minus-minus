#include "global.h"
#include "libgraphics.h"
#include "file.h"

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

/**
 * 计算传入字符的结束坐标
 * 传入开始的坐标与中间的字符串
 * */
RCNode endPos(RCNode startPos, char *str)
{
    RCNode nextPos = startPos;
    char *p = str;
    char *pp = p;
    char *enter = p;
    int isEnter = 0;
    while(*pp)
    {
        if(*pp=='\n')
        {
        	isEnter = 1;
            enter = pp;   //定位换行符位置
            nextPos.row ++;
        }
        pp ++;
    }
    if(isEnter)
    {
    	nextPos.column = strlen(enter);   //最后一行的列数为换行符后字符串的字数
	}else
	{
		nextPos.column += strlen(enter);  //列数为原列数加字符串长度 
	}
    
    return nextPos;
}

/**
 * 带警告的malloc
 */ 
void *mallocDIY(size_t size) {
    void *t = malloc(size);
    if (t == NULL) {
        MessageBoxA(NULL,"内存不足，程序即将关闭","Notepad--",MB_OK | MB_ICONWARNING | MB_TASKMODAL);
        exitApplication();
        return;
    } else {
        return t;
    }
}