/*
    主要包括复制 剪切 粘贴功能
    这一部分应该要和Windows剪贴板进行交互
*/
#include <stdio.h>
#include <Windows.h>
#include "global.h"
#include "edit.h"
#include "file.h"
#include "strlib.h"
#include <string.h>

/*
    根据传入的起始行列坐标与终止行列坐标进行复制
*/
void copyText()
{   
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();    
    char *cur = getCurrentString();  
    // printf("COPY1:%s\n", cur);
    int start, end;
    start = numofFormerWords(startSelect);   //计算复制字符的起始位置，结束位置
    end = numofFormerWords(endSelect);
    if (start > end) {
        int t = start;
        start = end;
        end = t;
    }
    if (start == end) return;   //选择空字符串则不复制
    char *copystr = (char *) malloc(sizeof(char) * (end - start + 1));   //定义需要复制的文本
    int length = end - start;
    //得到选中复制文本
    for (int i = 0; i < length; i++) {
        copystr[i] = cur[start + i];
    }
    copystr[length] = '\0';		//设置复制文本结束符（文本计数从1开始）
    // printf("COPY:%d %d %s\n", start, end, copystr);
    
    // printf("COPY2:%s\n", cur);
    //与系统剪切板进行交互
    if (!OpenClipboard(NULL)|| !EmptyClipboard())
    {   
        printf("打开或清空剪切板出错！\n");
        return ;   
    }
    HGLOBAL hMen;
    hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(copystr)+1)*sizeof(TCHAR)));    // 分配全局内存    
    // 把数据拷贝到全局内存中
    LPSTR lpStr = (LPSTR)GlobalLock(hMen);    // 先锁住内存区
    memcpy(lpStr, copystr, ((strlen(copystr))*sizeof(TCHAR)));    // 内存复制
    lpStr[strlen(copystr)] = (TCHAR)0;    // 设置字符结束符
    GlobalUnlock(hMen);   // 释放锁
    SetClipboardData(CF_TEXT, hMen);    // 把内存中的数据放到剪切板上
    CloseClipboard();  //关闭剪切板
    free(copystr);
    // printf("COPY3:%s\n", cur);
}

/*
    根据传入的起始位置粘贴文本内容（即需要修改对应的文件）
*/
void pasteText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    // int start, end, length;
    // start = numofFormerWords(startSelect);
    // end = numofFormerWords(endSelect);
    // if (start > end) {
    //     int t = start;
    //     start = end;
    //     end = t;
    // }

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }
    
    // length = end - start;  //计算要覆盖的字符的起始位置，终止位置，长度

    //与系统剪切板交互读取其中内容并储存在pasteText当中
    char *pasteText;
    if(!OpenClipboard(NULL))
    {
        printf("打开剪切板出错！\n");
        return ;   
    }
    HGLOBAL hMem = GetClipboardData(CF_TEXT);
    if(hMem)
    {
        //获取字符串
        LPSTR lpStr = (LPSTR)GlobalLock(hMem);
        if(lpStr)
        {   
            pasteText = (char *) malloc(sizeof(char) * (strlen(lpStr) + 1));
            strcpy(pasteText, lpStr);   //将剪切板的内容拷贝到字符串当中
            GlobalUnlock(hMem);   //释放内存锁
        }
    }
    //关闭剪切板
    CloseClipboard();

    //对原文本进行粘贴修改
    // char *curText = getCurrentString();
    /*char *cur = curText;
    char behindText[MAX];   //定义被选中文本后的字符串
    cur += (end+1);    //指针指向被选中文本的之后的第一个字符
    strcpy(behindText, cur);
    curText[start-1] = '\0';     //设置选择文本之前的字符串结束符
    strcat(curText, pasteText);    //将选中文本之前字符串连接粘贴文本
    strcat(curText, behindText);    //将粘贴后的文本与选中文本之后的字符串相连
    */
    // string s;
    // s = Concat(SubString(curText, 0, start - 1), pasteText);
    // //修改currentString 
	// setCurrentString(Concat(s,SubString(curText, end+1, StringLength(curText))));
	// free(s);
    deleteSelectString();
    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);
    addString(pasteText);

    //获得末端的行列位置
    int i = numofFormerWords(startSelect), j = i + strlen(pasteText);
    string s = getCurrentString();
    int lens = strlen(s);
    while (i < lens && i < j) {
        if (s[i] == '\n') {
            startSelect.row++;
            startSelect.column = 1;
        } else startSelect.column++;
        i++;
    }
    
    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);

    free(pasteText);
}

/*
    根据传入的起始行列坐标与终止行列坐标进行剪切
    即需要先复制这段内容，再将其删除
*/
void shearText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    copyText();   //先对选中内容进行复制

   /* //删除选中内容
    int start, end, length;
    start = numofFormerWords(startSelect);
    end = numofFormerWords(endSelect) - 1;
    length = end - start;
    char *curText = getCurrentString();
   char behindText[MAX];   //定义被选中文本后的字符
    char *cur = curText;
    cur += (end+1);    //指针指向被选中文本后的第一个字符
    strcpy(behindText, cur);
    curText[start-1] = '\0';    //设置选择文本之前的字符串结束符
    strcat(curText, behindText);    //连接前后字符串，将中间删除
    */
    //将修改后的文本输入
	deleteSelectString();
    
    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }

    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);
    
}
