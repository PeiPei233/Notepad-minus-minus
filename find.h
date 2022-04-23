/*
    主要实现查找替换功能
*/
#ifndef __FIND_H__
#define __FIND_H__

#include "global.h"

/*
    根据传入的字符串查找从当前光标位置开始的下一个匹配的字符串
    查找成功后更新选择范围为下一个匹配的字符串，并更新光标位置为选择范围末端，如果光标不在窗口内则更新窗口位置
*/
void findText(char *src);

/*
    根据传入的两个字符串（这里假设src为原先的字符串, tar为替换成的字符串）进行替换操作
    替换完成后刷新显示，并更新选择范围为替换后的字符串，且更新光标位置为选择位置末端
*/
void replaceText(char *src, char *tar);

#endif