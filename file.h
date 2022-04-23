/*
    主要实现文件交互的相关功能
*/
#ifndef __FILE_H__
#define __FILE_H__

#include "global.h"
/*
    打开一个文件
    这个应该要和Windows交互
    打开后更新窗口左上角坐标
    更新currentString字符串
*/
void openFile();

/*
    初始化缓存文件
*/
void initCache();

/*
    保存当前打开的文件
    如果是新建的文件 要和Windows交互选择打开位置
    否则就存在之前的位置
*/
void saveFile();

/*
    获取当前的显示的字符串
*/
char *getCurrentString();

/*
    自定义传入的参数 如窗口左上角的行列数等 根据传入参数从缓存文件中更新currentString
*/
void updateCurrentString();

/*
    根据传入的字符更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addChar(char ch);

/*
    根据传入的字符串更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addString(char *src);

/*
    根据当前光标位置，删除光标前的一个字符（在currentString与缓存文件中）
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void deleteChar();

/*
    根据当前选中的范围，删除选中的字符串（在currentString与cacheFile中）
    并更新光标以及选中范围的行列坐标（如有必要也更新窗口左上角位置）
*/
void deleteSelectString();

#endif