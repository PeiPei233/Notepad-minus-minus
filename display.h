/*
    主要包括文本的绘制 光标绘制 查找替换窗口的绘制 格式字体选择的绘制
*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "global.h"

/**
 * 获取查找窗口显示状态
 */ 
int getFindDisplayState();

/**
 * 获取替换窗口显示状态
 */ 
int getReplaceDisplayState();

/**
 * 获取右键菜单显示状态
 */ 
int getContextMenuDisplayState();

/**
 * 更改查找窗口显示状态
 */ 
void setFindDisplayState(int newFindDisplayState);

/**
 * 更改替换窗口显示状态
 */ 
void setReplaceDisplayState(int newReplaceDisplayState);

/**
 * 更改右键菜单显示状态
 */ 
void setContextMenuDisplayState(int newContextMenuDisplayState);

/**
 * 获取文本区域显示状态
 */
int getTextDisplayState();

/**
 * 获取当前显示的总行数
 */ 
int getTotalDisplayRow();

/**
 * 更新当前显示的总行数
 */ 
void updateTotalDisplayRow();

/**
 * 设置右键菜单的左上角位置
 */ 
void setContextMenuXY(double x, double y);

/**
 * 显示窗口内容
 */ 
void display();

/**
 * 将窗口的(x, y)像素坐标转换为行列坐标
 */ 
RCNode XYtoRC(int x, int y);

/**
 * 用于处理键盘按下时的快捷键
 * key      按下的按键
 * isShift  是否按下Shift
 * isCtrl   是否按下Ctrl
 * isTyping 是否在输入状态
 */ 
void processShortcutKey(int key, int isShift, int isCtrl, int isTyping);

/**
 * 若光标不在窗口中，则移动窗口使光标在窗口中
 */ 
void setCursorInWindow();

#endif