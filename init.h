/*
    该文件主要包括打开程序时的初始化图形界面，包括菜单，文本显示等等。
    记得修改对应的同名头文件，再加到main的include里
*/

#ifndef __INIT_H__
#define __INIT_H__

//主要需要实现以下函数

/*
    初始化各种自定义颜色
*/
void initColor();

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