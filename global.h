#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//行列坐标表示
typedef struct {
    int line;  //行位置
    int column;  //列位置
}LCNode;

extern double winHeight, winWidth;	//窗口高度和宽度

/*
    获取目前窗口左上角的行列位置
*/
LCNode getWindowCurrentLC();

/*
    更改目前窗口左上角的行列位置
*/
void setWindowCurrentLC();

/*
    根据传入的参数设置光标位置
*/
void setCursorLC();

/*
    获取光标位置
*/
LCNode getCursorLC();

/*
    获取选择范围起始位置
*/
LCNode getSelectStartLC();

/*
    获取选择范围终止位置
*/
LCNode getSelectEndLC();

/*
    设置选择范围起始位置
*/
void setSelectStartLC();

/*
    设置其实范围终止位置
*/
void setSelectEndLC();

/*
    判断光标位置是否在窗口内
*/
int cursorInWindow();

/*
    更改光标位置使其能够展示在窗口内
    如果光标已经在窗口内则不调整
*/
void setCursorInWindow();

#endif