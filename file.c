/* 首先要确定系统文件位置，初始化CACHE1和CACHE2;(建议就在这里新建一个cache文件夹) 
   其次打开后即需要立刻初始化缓存，即调用initCache()
   再次winrow 和wincolumn需要调用global的函数，不知咋的找不到最新的 
*/
#include <stdio.h>
#include "file.h"
#include <windows.h>
#include <Commdlg.h>
#include <string.h>

#define _N 1000010
#define CACHE1  "C:\\Users\\29431\\Documents\\GitHub\\UnableToCount\\myCache\\cache1.txt"
#define CACHE2	"C:\\Users\\29431\\Documents\\GitHub\\UnableToCount\\myCache\\cache2.txt"
static char currentString[_N];
static FILE *currentFile;  //当前文件 
static FILE *cacheFile1;    //缓存文件 
static FILE *cacheFile2;
OPENFILENAME ofn;
// a another memory buffer to contain the file name
char szFile[100];
static int isSaved;    //是否保存 
static int isCreated;  //是否已经确定保存位置 
static int winrow; 
static int wincolumn;     //这地方应该需要其他地方的数据，表示一个窗口有几行几列 
/*
    打开一个文件
    这个应该要和Windows交互
    打开后更新窗口左上角坐标
    更新currentString字符串
*/
void openFile() {
	initCache();
	    // open a file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn))
		currentFile=fopen(ofn.lpstrFile,"r+");
	else
	    MessageBox(NULL, "open failed", NULL, MB_OK);  //打开错误 
	fileWrite(currentFile,cacheFile1);            //更新至缓存中 
	fileWrite(currentFile,cacheFile2);
	isSaved=0;
	isCreated=1;
}

/*
    新建一个文件
    对于新建的文件，在保存时要选择存放位置
*/
void createFile() {
	// create a file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetSaveFileName(&ofn))
		currentFile=fopen(ofn.lpstrFile,"w+");
	else
	    MessageBox(NULL, "create failed", NULL, MB_OK);  //创建错误 
    isSaved=0;
    isCreated=1;
    initCache();
}

void fileWrite(FILE* fp1,FILE* fp2){
	char ch;
	while(!feof(fp1)){
		ch=fgetc(fp1);
		if(ch!=EOF)
			fputc(ch,fp2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);	
	fseek(currentFile,0L,0);	
}

/*
    初始化缓存文件
*/
void initCache() {
	cacheFile1=fopen(CACHE1,"w+");
	cacheFile2=fopen(CACHE2,"w+");
	isCreated=0;
}

/*
    保存当前打开的文件
    如果是新建的文件 要和Windows交互选择打开位置
    否则就存在之前的位置
*/
void saveFile() {
	isSaved=1;
	if(isCreated)
		fileWrite(cacheFile2,currentFile);
	else{
		ZeroMemory(&ofn, sizeof(ofn));
	    ofn.lStructSize = sizeof(ofn);
	    ofn.hwndOwner = NULL;
	    ofn.lpstrFile = szFile;
	    ofn.lpstrFile[0] = '\0';
	    ofn.nMaxFile = sizeof(szFile);
	    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	    ofn.nFilterIndex = 1;
	    ofn.lpstrFileTitle = NULL;
	    ofn.nMaxFileTitle = 0;
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	    if (GetSaveFileName(&ofn)){
			currentFile=fopen(ofn.lpstrFile,"w+");
			fileWrite(cacheFile2,currentFile);
		}
		else
			MessageBox(NULL, "create failed", NULL, MB_OK);  //保存错误 
		
	}
}

/*
    锟斤拷取锟斤拷前锟侥憋拷锟斤拷状态
    锟角否保达拷
*/
int getSaveState() {
	return isSaved;
}

/*
    获取当前的显示的字符串
*/
char *getCurrentString() {
	int i=0;char ch;
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);	
	while(!feof(cacheFile2)){
		ch=fgetc(cacheFile2);
		if(ch!=EOF)
		currentString[i++]=ch;
	}
	currentString[i]='\0';
	return currentString;
}

/*
    自定义传入的参数 如窗口左上角的行列数等 根据传入参数从缓存文件中更新currentString       哇兄弟，你这currentstring也不是全局变量啊，更新啥嘞 
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    根据传入的字符更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addChar(char ch) {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1
    RCNode cursor = getCursorRC();
    RCNode windowCursor=getWindowCurrentRC();
    int row=1,column=1;char chx;
    while(row<cursor.row||column<cursor.column){        
    	chx=fgetc(cacheFile2);
    	fgetc(cacheFile1);
    	if(chx=='\n')
    		row++;
    	else
    		column++;
	} 
	fputc(ch,cacheFile2);
	while(!feof(cacheFile1)){
		chx=fgetc(cacheFile1);
		if(ch!=EOF)fputc(chx,cacheFile2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);             //现在已经完成缓存的更新 
	if(cursor.column==windowCursor.column+wincolumn-1)
		windowCursor.column++;
	setWindowCurrentRC(windowCursor) ;	           //窗口移动 
	
	cursor.column++;
	setCursorRC(cursor);       
	getCurrentString();     //更新现在的字符串     
}

/*
    根据传入的字符串更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addString(char *src) {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 锟斤拷锟斤拷 
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
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		if(ch!=EOF)fputc(ch,cacheFile2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);             //现在已经完成缓存的更新 
	int x=cursor.column+strlen(src)-windowCursor.column-wincolumn+1;
	if(x>0)                                         //x>0 表示窗口应该移动的列数 其实就是把字符串长度加上，不需要换行 
		windowCursor.column+=x;
	setWindowCurrentRC(windowCursor);
	cursor.column+=strlen(src);
	setCursorRC(cursor); 
	getCurrentString();            //更新显示 
}

/*
    根据当前光标位置，删除光标前的一个字符（在currentString与缓存文件中）
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void deleteChar() {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 
    RCNode cursor = getCursorRC();
    int row=0,column=0;char ch;
    while(row<cursor.row||column<cursor.column){        
    	ch=fgetc(cacheFile1);
    	fputc(ch,cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	fgetc(cacheFile1);           // 利用与cache2一模一样的cache1完成操作（只需将cache1多读一位） 
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		if(ch!=EOF) fputc(ch,cacheFile2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);
	cursor.column--;
	setCursorRC(cursor);
	getCurrentString();
}

/*
    根据当前选中的范围，删除选中的字符串（在currentString与cacheFile中）
    并更新光标以及选中范围的行列坐标（如有必要也更新窗口左上角位置）
*/
void deleteSelectString() {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();                 
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
    while(row<endSelect.row||column<endSelect.column){        
    	ch=fgetc(cacheFile1);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	fgetc(cacheFile1);     //结束范围那个字符也应删去 
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		if(ch!=EOF) fputc(ch,cacheFile2);
	}
	setCursorRC(startSelect);
	setSelectEndRC(startSelect); 
	fseek(cacheFile1,0L,0);
	fseek(cacheFile2,0L,0);
	getCurrentString();
}









