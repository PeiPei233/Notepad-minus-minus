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
#include "storage.h"
#include "graphics.h"
#define _N 10000
static char *currentString;
static FILE *currentFile;  //当前文件 
OPENFILENAME ofn;
// a another memory buffer to contain the file name
static char szFile[100];       //存储文件名 
static char szFileTitle[20];
static int isSaved;    //是否保存         
static int isCreated;     //是否被创建 
/*
	初始化文件配置 , 一显示图形界面就要调用 
*/
void initFileConfig(){
	isSaved=1;
	isCreated=0;
	initStorage();
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
    ofn.lpstrFilter = "文本?件(*.txt)\0*.txt\0所有?件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileName(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"r+");
    }
	else{
	    MessageBox(NULL, "打开失败", NULL, MB_OK);  //打开错误
        return;
    } 
    int state;
    if(!(getTotalRow()==1&&getRowLength(1)==0)){            //若未被创建且当前显示字符串不为0时需弹出覆盖提示
        state = MessageBox(NULL,"打开后您当前的内容将被覆盖，确定打开吗？","友情提示",MB_OKCANCEL);
        if(state==1)    //ok
            ;
        else if(state==2){ //cancel
        	fclose(currentFile);
            return;
        }
    }
    initStorage();      //覆盖原先的内容 
	int i=0;
    unsigned int degree=1;
	int j=0;
	char ch;
	currentString=(char *)malloc(_N);
	while(!feof(currentFile)){
		ch=fgetc(currentFile);
		if(ch!=EOF)
			currentString[i++]=ch;
		if(i>_N*degree-2){
			char *temp;
			degree*=2;             //以指数级增长动态内存 
			temp=currentString;
			currentString=(char *)malloc(_N*degree);
			for(j=0;j<i;j++){
				currentString[j]=temp[j];
			}
			free(temp);
		}
	}
    if(degree>=2e9){                     //若大于2G,则无法打开
        MessageBox(NULL,"文件过大",NULL,MB_OK);
        free(currentString);
        return;
    }
	currentString[i]='\0';
	RCNode pos={1,1}; 
    addContent(BY_STRING,pos,currentString,0);
    free(currentString);
    fclose(currentFile);
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
    ofn.lpstrFilter = "文本?件(*.txt)\0*.txt\0所有?件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetSaveFileName(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"w+");
        fclose(currentFile);
    }
	else{
	    MessageBox(NULL, "创建失败", NULL, MB_OK);  //创建错误 
    }
    isSaved=1;
    isCreated=1;
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
	    ofn.lpstrFilter = "文本?件(*.txt)\0*.txt\0所有?件(*.*)\0*.*\0";
	    ofn.nFilterIndex = 1;
		 ofn.lpstrFileTitle = szFileTitle;
   		ofn.lpstrFileTitle[0] = '\0';
   	 	ofn.nMaxFileTitle = sizeof(szFileTitle);
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	    if (GetSaveFileName(&ofn)){
			if(currentFile=fopen(ofn.lpstrFile,"w+")){
				isCreated = 1;
                fclose(currentFile);
            }
		}
		else{
			MessageBox(NULL, "创建失败", NULL, MB_OK);  //创建错误 	
        }
	}
	if(!isSaved){         //如果未保存，写入currentFile中 
		currentFile=fopen(ofn.lpstrFile,"w+");     //此时ofn必有值 
		int i,row=getTotalRow();
        for(i=1;i<=row;i++){
            char *rowcontent=getRowContent(i);
            fputs(rowcontent,currentFile);
        }
		isSaved=1;
		fclose(currentFile);      //要保存 
	}
	else  ;                //也许在callback里设置？ 
}

/*
    获取当前的保存状态
    是否保存 
    刚创建视为已保存
*/
int getSaveState() {
	return isSaved;
}

/*
    设置保存状态
*/
void setSaveState(int newSaveState){
    isSaved=newSaveState;
}

/*
	返回当前文件名(不包括路径)
*/ 
char *getCurrentFileName(){
	if(isCreated){
		return ofn.lpstrFileTitle;
	} 
	else{
		return NULL;
	}
}
/*
    退出时若未保存则提供选项 选择是否保存更改 
*/

void exitApplication(){
	if(!isSaved){
		int a=MessageBox(NULL,"是否保存更改？","notepad--",MB_YESNOCANCEL);
		switch(a){
			case 6:             //是 
				saveFile();
			case 7:            //否 
				ExitGraphics();
				break;
			case 2:           //取消 
				break;
		}
	}
	else{
		ExitGraphics();
	}
}








