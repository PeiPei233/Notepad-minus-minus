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

static FILE *currentFile;  //当前文件 
OPENFILENAME ofn;
// a another memory buffer to contain the file name
static char szFile[512];       //存储文件名 
static char szFileTitle[512];
static int isSaved;    //是否保存         
static int isCreated;     //是否被创建 
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
    if (GetOpenFileName(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"r+");
    }
	else{
	    // MessageBox(NULL, "打开失败", NULL, MB_OK);  //打开错误
        return;
    } 
    int state;
    if(!(getTotalRow()==1&&getRowLength(1)==0)){            //若未被创建且当前显示字符串不为0时需弹出覆盖提示
        state = MessageBox(NULL,"打开后您当前的内容将被覆盖，确定打开吗？","Notepad--",MB_OKCANCEL | MB_TASKMODAL);
        if(state==IDOK)    //ok
            ;
        else if(state==IDCANCEL){ //cancel
        	fclose(currentFile);
            return;
        }
    }
    initStorage();      //覆盖原先的内容
    unsigned int degree=0;
	char ch;
	RCNode cursor = {1, 1};
	while(!feof(currentFile)){
		ch=fgetc(currentFile);
		if(ch!=EOF)
			addContent(BY_CHAR,cursor,&ch,1);
		if (ch=='\n'){
			cursor.row++;
			cursor.column=1;
		}
		else
			cursor.column++;
		degree++;
		if(degree>=1<<31){                     //若大于2G,则无法打开
			MessageBox(NULL,"文件过大，请尝试用 VSCode 等应用打开 :)",NULL,MB_OK | MB_TASKMODAL);
			initStorage();
			return;
		}
	}
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
    ofn.lpstrFilter = "文本文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0";
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
	    // MessageBox(NULL, "创建失败", NULL, MB_OK);  //创建错误 
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
		strcpy(ofn.lpstrFile, "*.txt");
	    ofn.nMaxFile = sizeof(szFile);
	    ofn.lpstrFilter = "文本文档(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0";
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

static char s[1010];

/*
    退出时若未保存则提供选项 选择是否保存更改 
*/
void exitApplication(){
	if(!isSaved){
		if (ofn.lpstrFile == NULL) {
			sprintf(s, "是否要保存对 无标题 的更改？");
		} else sprintf(s, "是否要保存对 %s 的更改？", ofn.lpstrFileTitle);
		int state=MessageBox(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
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








