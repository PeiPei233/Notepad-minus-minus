/*
    主要实现文件交互的相关功能
*/
/*
 每个文件编辑是会附带两个cache文件，分别存储上一步操作后和当前操作后 
 当前字符串存在数组中 
*/
#include <stdio.h>
#include "file.h"

#include <string.h>

#define _N 1000010
#define CACHE1
#define CACHE2
#define FILELOCATION
static char currentString[_N];
static FILE *currentFile;  //当前打开的文件
static FILE *cacheFile1;    //缓存文件
static FILE *cacheFile2;
static int isSaved;     //是否保存
static int winrow= GetWindowHeight() / getFontHeight();
static int wincolumn= GetWindowWidth() / 
/*
    打开一个文件
    这个应该要和Windows交互
    打开后更新窗口左上角坐标
    更新currentString字符串
*/
void openFile() {
	currentFile=fopen(FILELOCATION,"r+");
	fileWrite(currentFile,cacheFile1);            //要求缓存文件首先打开 
	fileWrite(currentFile,cacheFile2);
	isSaved=0;
}

/*
    新建一个文件
    对于新建的文件，在保存时要选择存放位置
*/
void createFile() {
    currentFile=fopen(,"w+");
    isSaved=0;
}

void fileWrite(FILE* fp1,FILE* fp2){
	while(!feof(fp1)){
		ch=fgetc(fp1);
		fputc(ch,fp2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);	
}

/*
    初始化缓存文件
*/
void initCache() {
	
}

/*
    保存当前打开的文件
    如果是新建的文件 要和Windows交互选择打开位置
    否则就存在之前的位置
*/
void saveFile() {
	isSaved=1;
	fileWrite(cacheFile2,currentFile);
}

/*
    获取当前的保存状态
    是否保存
*/
int getSaveState() {
	return isSaved;
}

/*
    获取当前的显示的字符串
*/
char *getCurrentString() {
	RCNode windowRC=getWindowCurrentRC();
	int row=0,column=0;
	char ch;int i=0;
    while(row<windowRC.row||column<windowRC.column){        //移动到左上角 
    	ch=fgetc(cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	row=1,column=1;
	while(!feof(cacheFile2) && (row<=winrow)){
		ch=fgetc(cacheFile2);
		if(ch=='\n'||column==windowRC.column+wincolumn){    //换行或者超出显示范围 
			row++;column=0;
			while(column<windowRC.column){
				fgetc(cacheFile2);
				column++;
			}
		}
		else{
			column++;
		}
		
		currentString[i++]=ch;
	}
	fseek(cacheFile2,0L,0);          //回到首部 
	currentString[i]='\0';      //我有个问题，这里的数组还能保存吗 
	return currentString;
}

/*
    自定义传入的参数 如窗口左上角的行列数等 根据传入参数从缓存文件中更新currentString      额，我觉得没啥必要吧 
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    根据传入的字符更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）          应该是在之后加一个吧 
*/
void addChar(char ch) {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 更新 
    RCNode cursor = getCursorRC();
    RCNode windowCursor=getWindowCurrentRC();
    int row=0,column=0;char chx;
    while(row<cursor.row||column<cursor.column){        
    	chx=fgetc(cacheFile2);
    	if(chx=='\n')
    		row++;
    	else
    		column++;
	} 
	fputc(ch,cacheFile2);
	if(cursor.column==windowCursor.column+wincolumn-1)
		windowCursor.column++;
	setWindowCurrentRC(windowCursor) ;	           //换行符似乎不好考虑 
	
	cursor.column++;
	setCursorRC(cursor);       
	fseek(cacheFile2,0L,0);
	getCurrentString();          // 也许在调用的地方用更好一些？ 
}

/*
    根据传入的字符串更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addString(char *src) {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 更新 
    RCNode cursor = getCursorRC();
    RCNode windowCursor=getWindowCurrentRC();
    int row=0,column=0;char ch;
    while(row<cursor.row||column<cursor.column){        
    	ch=fgetc(cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	fputs(src,cacheFile2);
	int x=cursor.column+strlen(src)-windowCursor.column-wincolumn+1;
	if(x>0)                                         //x>0 表明需要移动窗口 
		windowCursor.column+=x;
	setWindowCurrentRC(windowCursor);
	fseek(cacheFile2,0L,0);
	getCurrentString();
}

/*
    根据当前光标位置，删除光标前的一个字符（在currentString与缓存文件中）
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void deleteChar() {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 更新 
    RCNode cursor = getCursorRC();
    fclose(cacheFile2);
    cacheFile2=fopen(CACHE2,"w");
    int row=0,column=0;
    while(row<cursor.row||column<cursor.column){        
    	ch=fgetc(cacheFile1);
    	fputc(ch,cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	fgetc(cacheFile1);
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		fputc(ch,cacheFile2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);
	getCurrentString();
}

/*
    根据当前选中的范围，删除选中的字符串（在currentString与cacheFile中）
    并更新光标以及选中范围的行列坐标（如有必要也更新窗口左上角位置）
*/
void deleteSelectString() {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 更新 
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();                 //按理来讲应该是同一行的 
    fclose(cacheFile2);
    cacheFile2=fopen(CACHE2,"w");
    int row=0,column=0;char ch;
    while(row<startSelect.row||column<startSelect.column){        
    	ch=fgetc(cacheFile1);
    	fputc(ch,cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	int i;
	for( i=0;i<=endSelect.column-startSelect.column;i++)
		fgetc(cacheFile1);
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		fputc(ch,cacheFile2);
	}
	setCursorRC(startSelect);
	setSelectEndRC(startSelect); //起始位置和终点位置相同 
	fseek(cacheFile1,0L,0);
	fseek(cacheFile2,0L,0);
	getCurrentString();
}









