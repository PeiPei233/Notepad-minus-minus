#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "callback.h"

#define max 100000

//行列坐标表示
typedef struct {
    int row;  //行位置
    int column;  //列位置
}RCNode;

extern double winHeight, winWidth;	//窗口高度和宽度

/*
    获取目前窗口左上角的行列位置
*/
RCNode getWindowCurrentRC();

/*
    更改目前窗口左上角的行列位置
*/
void setWindowCurrentRC(RCNode newWindowCurrent);

/*
    根据传入的参数设置光标位置
*/
void setCursorRC(RCNode newCursor);

/*
    获取光标位置
*/
RCNode getCursorRC();

/*
    获取选择范围起始位置
*/
RCNode getSelectStartRC();

/*
    获取选择范围终止位置
*/
RCNode getSelectEndRC();

/*
    设置选择范围起始位置
*/
void setSelectStartLC(RCNode newStartSelect);

/*
    设置选择范围终止位置
*/
void setSelectEndLC(RCNode newEndSelect);

/*
    判断光标位置是否在窗口内
*/
int cursorInWindow(RCNode curCursor);

/*
    更改光标位置使其能够展示在窗口内
    如果光标已经在窗口内则不调整
*/
void setCursorInWindow();

/*
    实现将传入的行列数转化为总的字符数，方便在文件当中的读取
*/
int  numofFormerWords(RCNode curPosition);

/*
    将所有文本内容放入一个数组当中连续储存
*/
char* allText();

#endif