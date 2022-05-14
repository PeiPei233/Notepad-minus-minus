/* 
	�ǵ�һ��ʾ��Ҫ����initFileConfig() 
*/
#include <stdio.h>
#include "file.h"
#include <windows.h>
#include <Commdlg.h>
#include <string.h>
#include <strlib.h>

char *currentString;
static FILE *currentFile;  //��ǰ�ļ� 
OPENFILENAME ofn;
// a another memory buffer to contain the file name
char szFile[100];       //�洢�ļ��� 
static int isSaved;    //�Ƿ񱣴�         
static int isCreated;     //�Ƿ񱻴��� 
/*
	��ʼ���ļ����� , һ��ʾͼ�ν����Ҫ���� 
*/
void initFileConfig(){
	isSaved=1;
	isCreated=0;
}
/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
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
	    MessageBox(NULL, "open failed", NULL, MB_OK);  //�򿪴��� 
	int i=0;
	while(!feof(currentFile)){
		currentString[i++]=fgetc(currentFile);
	}
	currentString[i-1]='\0';
	isSaved=1;
	isCreated=1;
}

/*
    �½�һ���ļ�
    �����½����ļ����ڱ���ʱҪѡ����λ��
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
	    MessageBox(NULL, "create failed", NULL, MB_OK);  //�������� 
    isSaved=1;
    isCreated=1;
    currentString[0]='\0';
}

/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
*/
void saveFile() {
	isSaved=1;
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
			currentFile=fopen(ofn.lpstrFile,"w+");
		}
		else
			MessageBox(NULL, "create failed", NULL, MB_OK);  //������� 	
	}
	if(!isSaved){         //���δ���棬д��currentFile�� 
		int i=0,len=strlen(currentString);
		while(i<len)
			fputc(currentString[i++],currentFile);
		isSaved=1;
	}
	else  ;                //Ҳ����callback�����ã� 
}

/*
    ��ȡ��ǰ�ı���״̬
    �Ƿ񱣴�
*/
int getSaveState() {
	return isSaved;
}

/*
    ��ȡ��ǰ����ʾ���ַ���
*/
char *getCurrentString() {
	return currentString; 
}
/*
	���ص�ǰ�ļ���
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
	���õ�ǰ�ַ��� 
*/
void setCurrentString(char *update){
	free(currentString);
	currentString = update;
}
/*
    �Զ��崫��Ĳ��� �細�����Ͻǵ��������� ���ݴ�������ӻ����ļ��и���currentString       ���ֵܣ�����currentstringҲ����ȫ�ֱ�����������ɶ�� 
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    ���ݴ�����ַ�����currentString,
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void addChar(char ch) {
    RCNode cursor = getCursorRC();
    string s;
    int i = numofFormerWords(cursor);
    s = Concat(SubString(currentString, 0, i - 1), CharToString(ch));
    currentString = Concat(s, SubString(currentString, i, StringLength(currentString)));
    free(s);   
    isSaved=0;
}

/*
    ���ݴ�����ַ�������currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
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
    ���ݵ�ǰ���λ�ã�ɾ�����ǰ��һ���ַ�����currentString�뻺���ļ��У�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteChar() {
    RCNode cursor = getCursorRC();
    int i = numofFormerWords(cursor);
    currentString = Concat(SubString(currentString, 0, i - 2), SubString(currentString, i, StringLength(currentString)));
    isSaved=0;
}

/*
    ���ݵ�ǰѡ�еķ�Χ��ɾ��ѡ�е��ַ�������currentString��cacheFile�У�
    �����¹���Լ�ѡ�з�Χ���������꣨���б�ҪҲ���´������Ͻ�λ�ã�
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
    currentString = Concat(SubString(currentString, 0, i - 1), SubString(currentString, j, StringLength(currentString)));//���Կ������������genlib.h�е�freeblock���� 
    isSaved=0;
}









