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
 * 更改查找窗口显示状态
 */ 
void setFindDisplayState(int newFindDisplayState);

/**
 * 更改替换窗口显示状态
 */ 
void setReplaceDisplayState(int newReplaceDisplayState);

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
 * 显示窗口内容
 */ 
void display();

/**
 * 将窗口的(x, y)像素坐标转换为行列坐标
 */ 
RCNode XYtoRC(int x, int y);

#endif