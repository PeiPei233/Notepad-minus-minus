/*
    该文件主要包括打开程序时的初始化图形界面，包括菜单，文本显示等等。
    记得修改对应的同名头文件，再加到main的include里
*/

#include <stdio.h>
#include "global.h"
#include "display.h"
#include "libgraphics.h"
#include "imgui.h"
//主要需要实现以下函数

/*
    初始化各种自定义颜色
*/
void initColor() {
    DefineColor("Button Gray", 243 * 1./255, 243 * 1./255, 243 * 1./255);
    DefineColor("Button Hot Gray", 225 * 1./255, 225 * 1./255, 225 * 1./255);
    DefineColor("Menu Gray", 221 * 1./255, 221 * 1./255, 221 * 1./255);
    DefineColor("Menu Hot Gray", 198 * 1./255, 198 * 1./255, 198 * 1./255);
    DefineColor("Textbox Hot Blue", 0, 144 * 1./255, 241 * 1./255);
    DefineColor("Select Blue", 173 * 1./255, 214 * 1./255, 255 * 1./255);
}

/*
    初始化展示窗口
*/
void initDisplay() {
    initColor();
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);
    setMenuColors("Menu Gray", "Black", "Menu Hot Gray", "Black", 1);
    SetFont("Fira Code");
    display();
}