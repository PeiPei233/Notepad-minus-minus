/*
    主要实现文件交互的相关功能
*/

#include <stdio.h>

#define _N 1000010

char currentString[_N];
FILE *currentFile;  //当前打开的文件
FILE *cacheFile;    //缓存文件

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
void addChar();

/*
    根据传入的字符串更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addString();

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