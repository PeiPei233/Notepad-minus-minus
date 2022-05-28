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
#include "libgraphics.h"
#include "record.h"
#include "init.h"
#include "imgui.h"

static FILE *currentFile;  //当前文件 
OPENFILENAMEA ofn;
// a another memory buffer to contain the file name
static char szFile[512];       //存储文件名 
static char szFileTitle[512];
static int isSaved;    //是否保存         
static int isCreated;     //是否被创建 
static char s[1010];	//临时用
static int isProcessFile = 0;	//程序是否在读取/输出数据

/**
 * 获取程序处理文件的状态
 */ 
int getProcessFileState() {
	return isProcessFile;
}

/*
	初始化文件配置 , 一显示图形界面就要调用 
*/
void initFileConfig(){
	isSaved=1;
	isCreated=0;
    ZeroMemory(&ofn, sizeof(ofn));
}
/*
    打开一个文件
    这个应该要和Windows交互
    打开后更新窗口左上角坐标
    更新currentString字符串
*/
void openFile() {
	if (isProcessFile) return;
	isProcessFile = 1;

    int state;
    if (!isSaved) {	//未保存
		if (!isCreated) {
			sprintf(s, "是否要保存对 无标题 的更改？");
		} else sprintf(s, "是否要保存对 %s 的更改？", ofn.lpstrFileTitle);
		int state=MessageBoxA(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
		switch(state){
			case IDYES:             //是 
				saveFile();
				break;
			case IDNO:            //否 
				break;
			case IDCANCEL:           //取消 
				isProcessFile = 0;
				return;
		}
	}
	
	char szFile2[512];
	char szFileTitle2[512];
	
	strcpy(szFile2, szFile);
	strcpy(szFileTitle2, szFileTitle);

	    // open a file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"r+");
		if (!currentFile) {
			MessageBoxA(NULL,"打开失败","Notepad--",MB_OK | MB_TASKMODAL);
			strcpy(szFile, szFile2);
			strcpy(szFileTitle, szFileTitle2);
			isProcessFile = 0;
			return;
		}
    }
	else{
	    // MessageBox(NULL, "打开失败", NULL, MB_OK);  //打开错误
		strcpy(szFile, szFile2);
		strcpy(szFileTitle, szFileTitle2);
		isProcessFile = 0;
        return;
    } 

	fseeko64(currentFile, 0, SEEK_END);
    _off64_t degree = ftello64(currentFile);
	if(degree>=1ll<<31){                     //若大于2G,则警告用户
		int state = MessageBoxA(NULL,
		"文件过大！\n继续用 Notepad-- 打开可能会因为软件崩溃闪退而导致您的辛苦白费\n为了您的数据安全，建议使用 VSCode 等编辑器打开:)\n您确认要继续用 Notepad-- 打开吗？",
		NULL,
		MB_OKCANCEL | MB_TASKMODAL | MB_ICONWARNING);
		if (state==IDCANCEL) {
			strcpy(szFile, szFile2);
			strcpy(szFileTitle, szFileTitle2);
			isProcessFile = 0;	
			return;
		}
	}
	fseeko64(currentFile, 0, SEEK_SET);

	initStorage();
	InitGUI();

	setCursorRC((RCNode) {1, 1});
	setSelectStartRC((RCNode) {1, 1});
	setSelectEndRC((RCNode) {1, 1});
	setWindowCurrentRC((RCNode) {1, 1});

	char *fileText = (char *) mallocDIY(sizeof(char) * (degree + 10));
	unsigned long long n=0;
	char ch;
	while(ch = fgetc(currentFile), ch != EOF){
		fileText[n++] = ch;
	}
	fileText[n] = 0;
	addContent(BY_STRING, (RCNode) {1, 1}, fileText, 0);
    fclose(currentFile);
	free(fileText);
	isSaved=1;
	isCreated=1;
	isProcessFile = 0;
}


/*
    新建一个文件
    对于新建的文件，在保存时要选择存放位置
*/
void createFile() {
	if (isProcessFile) return;
	if (!isSaved) {	//未保存
		if (!isCreated) {
			sprintf(s, "是否要保存对 无标题 的更改？");
		} else sprintf(s, "是否要保存对 %s 的更改？", ofn.lpstrFileTitle);
		int state=MessageBoxA(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
		switch(state){
			case IDYES:             //是 
				saveFile();
			case IDNO:            //否 
				initApplication();
				break;
			case IDCANCEL:           //取消 
				break;
		}
	} else {
		initApplication();
	}
}

/*
    保存当前打开的文件
    如果是新建的文件 要和Windows交互选择打开位置
    否则就存在之前的位置
*/
void saveFile() {
	isProcessFile = 1;
	if(!isCreated)
	{
		ZeroMemory(&ofn, sizeof(ofn));
	    ofn.lStructSize = sizeof(ofn);
	    ofn.hwndOwner = NULL;
	    ofn.lpstrFile = szFile;
		strcpy(ofn.lpstrFile, "*.txt");
	    ofn.nMaxFile = sizeof(szFile);
	    ofn.lpstrFilter = "文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0";
	    ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = szFileTitle;
   		ofn.lpstrFileTitle[0] = '\0';
   	 	ofn.nMaxFileTitle = sizeof(szFileTitle);
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	    if (GetSaveFileNameA(&ofn)){
			//如果输入的文件名最后没有.txt则补上
			if (strlen(ofn.lpstrFileTitle) < 4 || strcmp(ofn.lpstrFileTitle + strlen(ofn.lpstrFileTitle) - 4, ".txt") != 0) {
				strcat(ofn.lpstrFileTitle, ".txt");
				strcat(ofn.lpstrFile, ".txt");
			}
			if(currentFile=fopen(ofn.lpstrFile,"w+")){
				isCreated = 1;
                fclose(currentFile);
            }
		}
		else{
			// MessageBox(NULL, "创建失败", NULL, MB_OK);  //创建错误 	
        }
	}
	//写入currentFile中 
	currentFile=fopen(ofn.lpstrFile,"w+");     //此时ofn必有值 
	int i,row=getTotalRow();
	for(i=1;i<=row;i++){
		char *rowcontent=getRowContent(i);
		int len=getRowLength(i);
		for(int j=0;j<len;j++){
			fputc(rowcontent[j],currentFile);
		}
	}
	isSaved=1;
	fclose(currentFile);
	isProcessFile = 0;

}

/*
	另存为 
	如果此时文件不是临时写的，则同时保存当时文件 
*/
void saveAsFile(){
	isProcessFile = 1;
	if (!isCreated) {
		saveFile();
		isProcessFile = 0;
		return;
	}
	OPENFILENAMEA ofn2;
	char szFile2[512];
	FILE *anotherFile;
	int flag=0;
    ZeroMemory(&ofn2, sizeof(ofn2));        //类似的操作 
    ofn2.lStructSize = sizeof(ofn2);
    ofn2.hwndOwner = NULL;
    ofn2.lpstrFile = szFile2;
    ofn2.lpstrFile[0] = '\0';
    ofn2.nMaxFile = sizeof(szFile2);
    ofn2.lpstrFilter = "文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0";
    ofn2.nFilterIndex = 1;
    ofn2.lpstrInitialDir = NULL;
    ofn2.Flags = OFN_PATHMUSTEXIST  | OFN_OVERWRITEPROMPT;
    if (GetSaveFileNameA(&ofn2)){
		//如果输入的文件名最后没有.txt则补上
		if (strlen(ofn2.lpstrFileTitle) < 4 || strcmp(ofn2.lpstrFileTitle + strlen(ofn2.lpstrFileTitle) - 4, ".txt") != 0) {
			strcat(ofn2.lpstrFileTitle, ".txt");
			strcat(ofn2.lpstrFile, ".txt");
		}
		anotherFile=fopen(ofn2.lpstrFile,"w+");
		if(isCreated){                   //如果此时文件不是临时写的，则同时保存当时文件 
			currentFile=fopen(ofn.lpstrFile,"w+");     //此时ofn必有值 
			flag=1;
			isSaved=1; 
		}
        int i,row=getTotalRow();
	   	for(i=1;i<=row;i++){
			char *rowcontent=getRowContent(i);
			int len=getRowLength(i);
			for(int j=0;j<len;j++){
				fputc(rowcontent[j],anotherFile);
				if(flag==1){
					fputc(rowcontent[j],currentFile);  //另存为后当前文件也保存
				}
			}
		}
		fclose(anotherFile);            
    }
	else{
	     //MessageBox(NULL, "创建失败", NULL, MB_OK);  //创建错误 
    }
	isProcessFile = 0;
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
		if (!isCreated) {
			sprintf(s, "是否要保存对 无标题 的更改？");
		} else sprintf(s, "是否要保存对 %s 的更改？", ofn.lpstrFileTitle);
		int state=MessageBoxA(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
		switch(state){
			case IDYES:             //是 
				saveFile();
				ExitGraphics();
			case IDNO:            //否 
				ExitGraphics();
				break;
			case IDCANCEL:           //取消 
				break;
		}
	}
	else{
		ExitGraphics();
	}
}








