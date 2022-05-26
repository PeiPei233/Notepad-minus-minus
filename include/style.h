/*
    该文件主要包括打开程序时的初始化图形界面，包括菜单，文本显示等等。
    记得修改对应的同名头文件，再加到main的include里
*/

#ifndef __STYLE_H__
#define __STYLE_H__

#include "strlib.h"

//主要需要实现以下函数
typedef struct {
    string fontFamily;
    int fontSize;
    double lineSpacing;
    string textColor;
    string backgroundColor;
}TextStyle;

/*
    初始化各种自定义颜色
*/
void initColor();

/**
 * 获取当前文字区域样式
 */ 
TextStyle getTextStyle();

/**
 * 设置当前文字区域样式
 */ 
void setTextStyle(TextStyle newTextStyle);

/**
 * 用十六进制颜色码定义新颜色
 */
int defineColorRGB(char *des, char *rgb);

//可添加其他初始化

/*
    初始化展示窗口
*/
void initDisplay();

/*
    刷新显示
*/
void display();

#endif