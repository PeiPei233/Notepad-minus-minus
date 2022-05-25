/*
    主要实现查找替换功能
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "find.h"
#include "libgraphics.h"
#include "storage.h"
#include "display.h"
#include "record.h"

static int restart=0; //记录是否重新扫描 
static int recordAll=0; //判断操作是否为replaceAll 
static int recordID;
/**
 * 根据传入的字符串从光标位置（选择范围前）查找上一个匹配的字符串
 * 查找成功后更新选择范围为下一个匹配的字符串，并更新光标位置为选择范围末端，如果光标不在窗口内则更新窗口位置
 */ 
int findLastText(const char *src) {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    RCNode cursor = getCursorRC();
    int len = strlen(src);      //查找字符串的长度
    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }
    cursor = startSelect;   //从选择范围的开头开始往上查找
    int row = cursor.row, col = cursor.column - 1;  //从光标前开始查
    string s = getRowContent(row);
    int find = 0;   //是否查找到
    for (row = cursor.row; row >= 1; row--) {
        if (row == cursor.row) col = cursor.column - 1;
        else col = getRowLength(row) - len + 1;
        string s = getRowContent(row);
        while (col >= 1) {
            int flag = 1;   //用于判断字符串是否相等
            for (int j = col - 1; j < col - 1 + len; j++) {   //col下标从1开始，要-1
                if (s[j] != src[j + 1 - col]) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {     
                find = 1;
                break;
            }
            col--;
        }
        if (find) {
            break;
        }
    }
    if (!find) {    //如果仍未找到，则从文章结尾再往前查
        for (row = getTotalRow(); row >= cursor.row; row--) {
            col = getRowLength(row) - len + 1;
            string s = getRowContent(row);
            while (col >= 1) {
                int flag = 1;   //用于判断字符串是否相等
                for (int j = col - 1; j < col - 1 + len; j++) {   //col下标从1开始，要-1
                    if (s[j] != src[j + 1 - col]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) {
                    find = 1;
                    break;
                }
                col--;
            }
            if (find) {
                break;
            }
        }
        if (!find) {    //如果还是没找到
            return 0;
        }
    }
    if (!find) return 0;
    //成功找到，设置选择范围
    setSelectStartRC((RCNode) {row, col});
    setSelectEndRC((RCNode) {row, col + len});
    setCursorRC((RCNode) {row, col + len});
    setCursorInWindow();
    return 1;
}

/*
    根据传入的字符串查找从当前光标位置开始的下一个匹配的字符串
    查找成功后更新选择范围为下一个匹配的字符串，并更新光标位置为选择范围末端，如果光标不在窗口内则更新窗口位置
*/
int findNextText(const char *src) {
	// printf("FIND:%s\n", src);
    RCNode startCursor = getSelectEndRC();  //从选择范围终点作为查找开始坐标
    int row=startCursor.row, column=startCursor.column;
    RCNode findCursor;
    int length = strlen(src);   //计算查找字符长度

    char *str;
	str = getRowContent(row); //获取当前行显示的内容
    int flag;
    str += column-1;
    int totalrow=getTotalRow();
    while(row<=totalrow){
        while(*str != 0)
        {   
            // printf("FIND:%s\n", ch);
            int i;
            flag = 1;
            //判断是否是查找字符串
            for(i=0; i<length&&*str!=0; i++)
            {
                if(*(str+i) != *(src+i))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag&&i==length)   //如果找到了则定位查找字符
            {
                printf("FIND:\n");
                findCursor.column = column;
                findCursor.row = row;
                setSelectStartRC(findCursor);   //设置选择开始坐标
                findCursor.column += length;
                setSelectEndRC(findCursor);   //设置选择结束坐标
                setCursorRC(findCursor);    //设置光标坐标
                setCursorInWindow();
                return 1;          
            }else  //不是查找内容则移动指针
            {
                    column ++;
                    str ++;
            }
        }
        row++;
        column=1;      //换行，需初始化column 
        str = getRowContent(row); //获取当前行显示的内容
    }
    //若未找到则返回开头继续查找
    row=1;column=1; 
    str=getRowContent(row);
    restart++;                     //记录重来的次数，为replaceAll做准备 
    while(row<=totalrow){
        while(*str != 0)
        {   
            // printf("FIND:%s\n", ch);
            int i;
            flag = 1;
            //判断是否是查找字符串
            for(i=0; i<length&&*str!=0; i++)
            {
                if(*(str+i) != *(src+i))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag&&i==length)   //如果找到了则定位查找字符
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
            }else  //不是查找内容则移动指针
            {
                    column ++;
                    str ++;
            }
        }
        row++;
        column=1;
        str = getRowContent(row); //获取当前行显示的内容
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
int replaceText(const char *src, const char *tar) {
    RCNode selectStart, selectEnd;
    selectStart = getSelectStartRC();
    selectEnd = getSelectEndRC();
	int findflag=1;
	//设定为start在前，end在后 
    if (selectStart.row > selectEnd.row || (selectStart.row == selectEnd.row && selectStart.column > selectEnd.column)) {
        RCNode t = selectStart;
        selectStart = selectEnd;
        selectEnd = t;
    }
    //获取选中内容并储存在selectStr中
    char *selectStr = getContent(selectStart,selectEnd);
 
    // printf("REPLACE_CMP:%s %s\n", selectStr, src);
    //判断是否是查找内容，不是则重新选择 
	if(!selectStr){
		if(!findNextText(src)){
			findflag=0;
		}
	}   
    else if(strcmp(selectStr, src))    
    {
        if(!findNextText(src)){
        	findflag=0;
		}
    }
    if(findflag){             //若能找到结果，替换 
	    printf("REPLACE:FIND!\n");
	    deleteContent(selectStart,selectEnd,1);     //删除源字符串 
	    addContent(BY_STRING,selectStart,tar,1);  //粘贴目的字符串 
	    
	    if(!recordAll){                               //记录操作，并为一步 若为replaceAll,则全部为一步 
	    	recordID=newRecordID();
		}
	    record(OP_DELETE,selectStart,src,recordID);
	    record(OP_ADD,selectStart,tar,recordID);
	    
	    setSelectStartRC(selectStart);    
	    selectStart.column+=strlen(tar);
	    setSelectEndRC(selectStart);              //设置选择范围 
	    setCursorRC(selectStart);
	    findNextText(src);
		free(selectStr);
		
	    return 1; 
	}
	else{				//若不能找到结果，返回0 
		return 0;
	}
}

/*
	替换全部 
*/ 
void replaceAll(const char *src,const char *tar){
	restart=0;
	recordAll=1;
	recordID=newRecordID();
	RCNode start=getSelectStartRC();
	RCNode now=getSelectStartRC();
	while(1){
        now=getSelectStartRC();
        if(!((restart==0&&RCcompare(now,start)>=0)||(restart==1&&RCcompare(start,now)>0))){
            break;
        }
        if(!replaceText(src,tar)){
        	break;
		}
	}
	restart=0;
	recordAll=0;      //恢复一般状态 
}
