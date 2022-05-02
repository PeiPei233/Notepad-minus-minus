#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent; //窗口左上角的行列坐标
static RCNode cursor;  //光标所在的行列坐标
static RCNode startSelect, endSelect;  //选择范围的起点/终点行列位置
int bigin, end;

/*
    用户获取目前窗口左上角的行列位置
*/
RCNode getWindowCurrentRC() 
{
    return windowCurrent;
}

/*
    用户更改目前窗口左上角的行列位置
*/
void setWindowCurrentRC(RCNode newWindowCurrent) 
{
    windowCurrent = newWindowCurrent;
}

/*
    根据传入的参数设置光标位置
*/
void setCursorRC(RCNode newCursor) 
{
    cursor = newCursor;
}

/*
    获取光标位置
*/
RCNode getCursorRC() 
{
    return cursor;
}

/*
    获取选择范围起始位置
*/
RCNode getSelectStartRC() 
{
    return startSelect;
}

/*
    获取选择范围终止位置
*/
RCNode getSelectEndRC() 
{
    return endSelect;
}

/*
    设置选择范围起始位置
*/
void setSelectStartRC(RCNode newStartSelect)
{
    startSelect = newStartSelect;
}

/*
    设置选择范围终止位置
*/
void setSelectEndRC(RCNode newEndSelect)
{
    endSelect = newEndSelect;
}

/*
    判断光标位置是否在窗口内
*/
int cursorInWindow(RCNode curCursor) 
{
    
}

/*
    更改光标位置使其能够展示在窗口内
    如果光标已经在窗口内则不调整
*/
void setCursorInWindow() 
{
    
}

/*
    根据传入的坐标计算在所有字符串当中的位置（第几个）
*/

int numofFormerWords(RCNode curPosition)
{
    char word_[max];
    char *word = word_;
	strcpy(word_, allText());
    int num=0;  //指定字符所处的位置
    int searchColumn=1;   //定义指针指向文本文件当中字符所位于的行列坐标
    int searchRow=0;
    int enterRow=0;   //定义换行符所在的该行的位置
    int isEnter=0;   //判断是否换行
    while(*word != '\0')
    {
        //当到达图形界面每行最大字符数或者遇到换行符时换行
        if(searchRow == 1<<30)  //maxn为一行能达到的最大字符数
        {
            searchColumn ++;
            searchRow = 0;
            isEnter = 0;
        }else if(*word == '\n')
        {
            enterRow = searchRow;
            searchColumn ++;
            searchRow = 0;
            isEnter = 1;
        }
        //坐标相等，找到指定位置
        if(searchColumn == curPosition.column && searchRow == curPosition.row)
        {
        	num ++;
            break;
        //当鼠标位于换行所处行的空白处，定位至该行末尾
        }else if(isEnter == 1)
        {
	    	if((searchColumn-1) == curPosition.column && curPosition.row >= enterRow)
	        {
	            break;
	        }
	        isEnter = 0;
	        searchRow ++;
			word ++;
			num ++;
        //其他情况指针后移，计数增加
		}else
        {
        	word ++;
        	searchRow ++;
        	num ++;
		}
    }
    return num;
}

/*
    将所有文本内容放入一个数组当中连续储存
*/
char* allText()
{
    FILE *fp;
    char colunmStr[max];    //定义暂时储存每行文本字符的数组
    char allText[max];  //定义储存所有文本的数组
    strcpy(allText, "");
    if(fp = fopen("currentFile.txt", "r"))
    {
        //分行进行读取并储存
        while(fgets(colunmStr, max, fp))
        {
            strcat(allText, colunmStr);
            strcpy(colunmStr,"");
        }
        fclose(fp);
        return allText;
    }else
    {
        printf("打开错误！");
        return NULL;
    }
}