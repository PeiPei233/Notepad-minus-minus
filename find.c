/*
    主要实现查找替换功能
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "find.h"

RCNode findStr[MAX];

/*
    根据传入的字符串查找从当前光标位置开始的下一个匹配的字符串
    查找成功后更新选择范围为下一个匹配的字符串，并更新光标位置为选择范围末端，如果光标不在窗口内则更新窗口位置
*/
int findText(char *src) {printf("FIND:%s\n", src);
    RCNode startCursor = getCursorRC();  //获取当前光标位置作为查找开始坐标
    int row=startCursor.row, column=startCursor.column;
    RCNode findCursor;
    int length = strlen(src);   //计算查找字符长度
    char *ch = (char*)malloc(sizeof(char)*MAX);
    strcpy(ch, getCurrentString());   //获取当前屏幕现实的内容
    char *ch_=ch;   //定义指针储存字符串初始位置
    int flag;
    int startW = numofFormerWords(startCursor);
    ch += startW;
    while(ch != NULL)
    {
        int i;
        flag = 1;
        //判断是否是查找字符串
        for(i=0; i<length; i++)
        {
            if(*(ch+i) != *(src+i))
            {
                flag = 0;
                break;
            }
        }
        if(flag)   //如果找到了则定位查找字符
        {
            findCursor.column = column;
            findCursor.row = row;
            if(!cursorInWindow(findCursor))  //如果不在窗口内则更改坐标位置
            {
                setCursorInWindow();
            }
            setSelectStartRC(findCursor);   //设置选择开始坐标
            findCursor.row += length;
            setSelectEndRC(findCursor);   //设置选择结束坐标
            setCursorRC(findCursor);    //设置光标坐标
            return 1;          
        }else  //不是查找内容则改变行列数，同时移动指针
        {
            ch ++;
            if(*ch != '\n')
            {
                row ++;
            }else
            {
                column ++;
            }
        }
    }
    
    //如果选中文本之后内容没有查找到，则返回文本开头查找，直到选中文本之前
    column = 0;
    row = 0;
    int k;
    for(k=0; k<startW-length; k++)
    {
        int i;
        flag = 1;
        //判断是否是查找字符串
        for(i=0; i<length; i++)
        {
            if(*(ch_+i) != *(src+i))
            {
                flag = 0;
                break;
            }
        }
        if(flag)   //如果找到了则定位查找字符
        {
            findCursor.column = column;
            findCursor.row = row;
            if(!cursorInWindow(findCursor))  //如果不在窗口内则更改坐标位置
            {
                setCursorInWindow();
            }
            setSelectStartRC(findCursor);   //设置选择开始坐标
            findCursor.row += length;
            setSelectEndRC(findCursor);   //设置选择结束坐标
            setCursorRC(findCursor);    //设置光标坐标
            return 1;          
        }else  //不是查找内容则改变行列数，同时移动指针
        {
            ch_ ++;
            if(*ch_ != '\n')
            {
                row ++;
            }else
            {
                column ++;
            }
        }
    }
    //如果遍历完显示文本仍然没有找到查找内容，设置返回0
    return 0;
}


/*
    根据传入的两个字符串（这里假设src为原先的字符串, tar为替换成的字符串）进行替换操作
    替换完成后刷新显示，并更新选择范围为替换后的字符串，且更新光标位置为选择位置末端
    如果当前选中的字符串不是src，则先查找光标后的下一个符合的src并选中，即先执行findText(src)
    否则如果当前选中的字符串就是src，则替换为tar并将选择范围改为替换后的字符串并更新光标位置为选择位置末端吗
*/
void replaceText(char *src, char *tar) {
    char *allStr = getCurrentString();    //获取所有文本
    RCNode selectStart, selectEnd;
    selectStart = getSelectStartRC();
    selectEnd = getSelectEndRC();
    int start = numofFormerWords(selectStart);    //获取选择范围的起始，终止位置
    int end = numofFormerWords(selectEnd) - 1;
    int length = end - start;

    //获取选中内容并储存在selectStr中
    char selectStr[MAX];
    char* str = allStr;
    str += start;
    strcpy(selectStr, str);
    selectStr[length+1] = '\0';

    //判断是否是查找内容，不是则重新选择    
    if(strcmp(selectStr, src))    
    {
        findText(src);
        replaceText(src, tar);
    }

    char behindText[MAX];   //定义被选中文本后的字符串
    str += (length+1);    //指针指向被选中文本的之后的第一个字符
    strcpy(behindText, str);
    allStr[start-1] = '\0';     //设置选择文本之前的字符串结束符
    strcat(allStr, tar);    //将选中文本之前字符串连接粘贴文本
    strcat(allStr, behindText);    //将粘贴后的文本与选中文本之后的字符串相连

    //将修改后的文本输入
    FILE *currentFile;  
    if(!(currentFile = fopen("./cache1.txt","w")))
    {
        printf("打开错误");
        return ;
    }
    fprintf(currentFile, "%s", allStr);  
    fclose(currentFile);
    display();
}