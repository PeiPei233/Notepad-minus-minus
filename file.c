/* 
	记得一显示就要调用initFileConfig() 
*/
#include <stdio.h>
#include "file.h"
#include <windows.h>
#include <Commdlg.h>
#include <string.h>
#include "strlib.h"
#include <stdlib.h>
#define _N 10000
#define Block 1000
char *currentString;
static FILE *currentFile;  //当前文件 
OPENFILENAME ofn;
// a another memory buffer to contain the file name
char szFile[100];       //存储文件名 
static int isSaved;    //是否保存         
static int isCreated;     //是否被创建 
/*
	初始化文件配置 , 一显示图形界面就要调用 
*/
void initFileConfig(){
	isSaved=1;
	isCreated=0;
	currentString=(char *)calloc(1,1);
}
/*
    打开一个文件
    这个应该要和Windows交互
    打开后更新窗口左上角坐标
    更新currentString字符串
*/
void openFile() {
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
	int i=0,degree=1;
	int j=0;
	char ch;
	currentString=(char *)malloc(_N);
	while(!feof(currentFile)){
		ch=fgetc(currentFile);
		if(ch!=EOF)
			currentString[i++]=ch;
		if(i>_N*degree-2){
			char *temp;
			degree+=3;
			temp=currentString;
			currentString=(char *)malloc(_N*degree);
			for(j=0;j<i;j++){
				currentString[j]=temp[j];
			}
			free(temp);
		}
	}
	currentString[i]='\0';
	isSaved=1;
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
    isSaved=1;
    isCreated=1;
    currentString[0]='\0';
}

/*
    保存当前打开的文件
    如果是新建的文件 要和Windows交互选择打开位置
    否则就存在之前的位置
*/
void saveFile() {
	if(isCreated)
		;
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
			if(currentFile=fopen(ofn.lpstrFile,"w+"))
				;
		}
		else
			MessageBox(NULL, "create failed", NULL, MB_OK);  //保存错误 	
	}
	if(!isSaved){         //如果未保存，写入currentFile中 
		fclose(currentFile);
		currentFile=fopen(ofn.lpstrFile,"w+");        //重新写，确保之前的没了 
		int i=0,len=strlen(currentString);
		while(i<len)
			fputc(currentString[i++],currentFile);
		isSaved=1;
		fclose(currentFile);      //要保存 
		currentFile=fopen(ofn.lpstrFile,"r+");
	}
	else  ;                //也许在callback里设置？ 
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
	return currentString; 
}
/*
	返回当前文件名
*/ 
char *getCurrentFileName(){
	if(isCreated){
		return ofn.lpstrFile;
	} 
	else{
		return NULL;
	}
}
/*
	设置当前字符串 
*/
void setCurrentString(char *update){
	free(currentString);
	currentString = update;
}
/*
    自定义传入的参数 如窗口左上角的行列数等 根据传入参数从缓存文件中更新currentString       哇兄弟，你这currentstring也不是全局变量啊，更新啥嘞 
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    根据传入的字符更新currentString,
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void addChar(char ch) {
    RCNode cursor = getCursorRC();
    string s;string sub;
    int i = numofFormerWords(cursor);
    s = Concat(SubString(currentString, 0, i - 1), CharToString(ch));
    sub = SubString(currentString, i, StringLength(currentString));
    free(currentString);                      //貌似我们所谓的FreeBlock这里只用到了free这样一个函数，这里就不采用别的了 
    currentString = Concat(s,sub);
    isSaved=0;
}

/*
    根据传入的字符串更新currentString与缓存文件
    并更新光标位置（如有必要也更新窗口左上角位置）        话说这个好像没有用到吧 
*/
void addString(char *src) {
    RCNode cursor = getCursorRC();
    string s;
    int i = numofFormerWords(cursor);
    s = Concat(SubString(currentString, 0, i - 1), src);
    currentString = Concat(s, SubString(currentString, i, StringLength(currentString)));
    free(s);  
    isSaved=0;
}

/*
    根据当前光标位置，删除光标前的一个字符（在currentString与缓存文件中）
    并更新光标位置（如有必要也更新窗口左上角位置）
*/
void deleteChar() {
    RCNode cursor = getCursorRC();
    int i = numofFormerWords(cursor);
    string sub1,sub2;
    sub1=SubString(currentString, 0, i - 2);
    sub2=SubString(currentString, i, StringLength(currentString));
    free(currentString);
    currentString = Concat(sub1,sub2);
    isSaved=0;
}

/*
    根据当前选中的范围，删除选中的字符串（在currentString与cacheFile中）
    并更新光标以及选中范围的行列坐标（如有必要也更新窗口左上角位置）
*/
void deleteSelectString() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    int i = numofFormerWords(startSelect);
    int j = numofFormerWords(endSelect);
    if (i > j) {
        int t = i;
        i = j;
        j = t;
    }
    string sub1,sub2;
	sub1= SubString(currentString, 0, i - 1);
	sub2=SubString(currentString, j, StringLength(currentString));
    free(currentString);
    currentString = Concat(sub1,sub2);//可以考虑在这里调用genlib.h中的freeblock函数 
    isSaved=0;
}









