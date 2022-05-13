#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent = (RCNode) {1, 1}; //窗口左上角的行列坐标
static RCNode cursor = (RCNode) {1, 1};  //光标所在的行列坐标
static RCNode startSelect = (RCNode) {1, 1}, endSelect = (RCNode) {1, 1};  //选择范围的起点/终点行列位置
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
    判断光标位置是否在窗口内
*/
int cursorInWindow(RCNode curCursor) 
{
    
}

/*
    更改光标位置使其能够展示在窗口内
    如果光标已经在窗口内则不调整
*/
void setCursorInWindow() 
{
    
}

/*
    根据传入的坐标计算在所有字符串当中的位置（第几个）
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
 * 获得第 row 行的总列数
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
 * 获得文件总行数
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