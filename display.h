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

void display();

/*
    画菜单 并根据菜单选项执行对应的函数和命令
    在没有打开任何文件时显示新建文件
*/
void drawMenu();

/*
    在范围内画文本，建议的参数包括左上角坐标(x, y) 矩形的长宽(w, h) 行高 是否需要自动换行 等等
*/
void drawTextArea();

/*
    绘制查找窗口 包括一个查找内容文本框和查找按钮等
    建议绘制在文件文本的右上角
    先绘制底层文本 在绘制查找窗口时先绘制一个白色矩形覆盖底下的文本，在绘制查找窗口
    绘制替换窗口同理
*/
void drawFindArea();

/*
    绘制替换窗口 包括两个文本框（分别是查找和替换）以及替换按钮等
*/
void drawReplaceArea();

#endif