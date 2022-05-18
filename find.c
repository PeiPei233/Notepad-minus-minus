/*
    主要实现查找替换功能
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "find.h"
#include "libgraphics.h"

/*
    根据传入的字符串查找从当前光标位置开始的下一个匹配的字符串
    查找成功后更新选择范围为下一个匹配的字符串，并更新光标位置为选择范围末端，如果光标不在窗口内则更新窗口位置
*/
int findText(char *src) {
	// printf("FIND:%s\n", src);
    RCNode startCursor = getCursorRC();  //获取当前光标位置作为查找开始坐标
    int row=startCursor.row, column=startCursor.column;
    RCNode findCursor;
    int length = strlen(src);   //计算查找字符长度

    char *ch;
	ch=getCurrentString(); //获取当前屏幕现实的内容
    char *ch_=ch;    //定义指针储存字符串初始位置
    int flag;
    int startW = numofFormerWords(startCursor);
    ch += startW;

    while(*ch != 0)
    {   
        // printf("FIND:%s\n", ch);
        int i;
        flag = 1;
        ///判断是否是查找字符串
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
            // printf("FIND:%s\n", ch);
            findCursor.column = column;
            findCursor.row = row;
            setSelectStartRC(findCursor);   //设置选择开始坐标
            findCursor.column += length;
            setSelectEndRC(findCursor);   //设置选择结束坐标
            setCursorRC(findCursor);    //设置光标坐标
            setCursorInWindow();
            return 1;          
        }else  //不是查找内容则改变行列数，同时移动指针
        {
            if(*ch != '\n')
            {
                column ++;
                ch ++;
            }else
            {   
                ch++;
                column = 1;
                row++;
            }
        }
    }
    
    //如果选中文本之后内容没有查找到，则返回文本开头查找，直到选中文本之前
    column = 1;
    row = 1;
    int k;
    for(k=0; k<startW; k++)
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
            // printf("FIND:%s\n", ch_);
            findCursor.column = column;
            findCursor.row = row;
            setSelectStartRC(findCursor);   //设置选择开始坐标
            findCursor.column += length;
            setSelectEndRC(findCursor);   //设置选择结束坐标
            setCursorRC(findCursor);    //设置光标坐标
            setCursorInWindow();
            return 1;          
        }else  //不是查找内容则改变行列数，同时移动指针
        {
            if(*ch_ != '\n')
            {
                column ++;
                ch_++;
            }else
            {
                ch_++;
                row ++;
                column = 1;
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
    int end = numofFormerWords(selectEnd);
    if (end < start) {
        int t = end;
        end = start;
        start = t;
    }
    int length = end - start;

    //获取选中内容并储存在selectStr中
    char *selectStr = (char *) malloc(sizeof(char) * (end - start + 1));
    char* str = allStr;
    // strcpy(selectStr, str + start);
    for (int i = 0; i < length; i++) {
        selectStr[i] = str[start + i];
    }
    selectStr[length+1] = '\0';
    // printf("REPLACE_CMP:%s %s\n", selectStr, src);
    //判断是否是查找内容，不是则重新选择    
    if(strcmp(selectStr, src))    
    {
        findText(src);
        return;
    }

    if (selectStart.row > selectEnd.row || (selectStart.row == selectEnd.row && selectStart.column > selectEnd.column)) {
        RCNode t = selectStart;
        selectStart = selectEnd;
        selectEnd = t;
    }
    
    // string s;
    // s = Concat(SubString(allStr, 0, start - 1), tar);
    // //修改currentString 
	// setCurrentString(Concat(s,SubString(allStr, end+1, StringLength(allStr))));
    // free(selectStr);
    printf("REPLACE:FIND!\n");
    deleteSelectString();
    setSelectStartRC(selectStart);
    setSelectEndRC(selectStart);
    setCursorRC(selectStart);
    addString(tar);

    findText(src);

}
