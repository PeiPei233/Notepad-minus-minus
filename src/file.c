/* 
	�ǵ�һ��ʾ��Ҫ����initFileConfig() 
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

static FILE *currentFile;  //��ǰ�ļ� 
OPENFILENAMEA ofn;
// a another memory buffer to contain the file name
static char szFile[512];       //�洢�ļ��� 
static char szFileTitle[512];
static int isSaved;    //�Ƿ񱣴�         
static int isCreated;     //�Ƿ񱻴��� 
static char s[1010];	//��ʱ��
static int isProcessFile = 0;	//�����Ƿ��ڶ�ȡ/�������

/**
 * ��ȡ�������ļ���״̬
 */ 
int getProcessFileState() {
	return isProcessFile;
}

/*
	��ʼ���ļ����� , һ��ʾͼ�ν����Ҫ���� 
*/
void initFileConfig(){
	isSaved=1;
	isCreated=0;
    ZeroMemory(&ofn, sizeof(ofn));
}
/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
*/
void openFile() {
	if (isProcessFile) return;
	isProcessFile = 1;

    int state;
    if (!isSaved) {	//δ����
		if (!isCreated) {
			sprintf(s, "�Ƿ�Ҫ����� �ޱ��� �ĸ��ģ�");
		} else sprintf(s, "�Ƿ�Ҫ����� %s �ĸ��ģ�", ofn.lpstrFileTitle);
		int state=MessageBoxA(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
		switch(state){
			case IDYES:             //�� 
				saveFile();
				break;
			case IDNO:            //�� 
				break;
			case IDCANCEL:           //ȡ�� 
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
    ofn.lpstrFilter = "�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"r+");
		if (!currentFile) {
			MessageBoxA(NULL,"��ʧ��","Notepad--",MB_OK | MB_TASKMODAL);
			strcpy(szFile, szFile2);
			strcpy(szFileTitle, szFileTitle2);
			isProcessFile = 0;
			return;
		}
    }
	else{
	    // MessageBox(NULL, "��ʧ��", NULL, MB_OK);  //�򿪴���
		strcpy(szFile, szFile2);
		strcpy(szFileTitle, szFileTitle2);
		isProcessFile = 0;
        return;
    } 

	fseeko64(currentFile, 0, SEEK_END);
    _off64_t degree = ftello64(currentFile);
	if(degree>=1ll<<31){                     //������2G,�򾯸��û�
		int state = MessageBoxA(NULL,
		"�ļ�����\n������ Notepad-- �򿪿��ܻ���Ϊ����������˶�������������׷�\nΪ���������ݰ�ȫ������ʹ�� VSCode �ȱ༭����:)\n��ȷ��Ҫ������ Notepad-- ����",
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
    �½�һ���ļ�
    �����½����ļ����ڱ���ʱҪѡ����λ��
*/
void createFile() {
	if (isProcessFile) return;
	if (!isSaved) {	//δ����
		if (!isCreated) {
			sprintf(s, "�Ƿ�Ҫ����� �ޱ��� �ĸ��ģ�");
		} else sprintf(s, "�Ƿ�Ҫ����� %s �ĸ��ģ�", ofn.lpstrFileTitle);
		int state=MessageBoxA(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
		switch(state){
			case IDYES:             //�� 
				saveFile();
			case IDNO:            //�� 
				initApplication();
				break;
			case IDCANCEL:           //ȡ�� 
				break;
		}
	} else {
		initApplication();
	}
}

/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
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
	    ofn.lpstrFilter = "�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0";
	    ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = szFileTitle;
   		ofn.lpstrFileTitle[0] = '\0';
   	 	ofn.nMaxFileTitle = sizeof(szFileTitle);
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	    if (GetSaveFileNameA(&ofn)){
			//���������ļ������û��.txt����
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
			// MessageBox(NULL, "����ʧ��", NULL, MB_OK);  //�������� 	
        }
	}
	//д��currentFile�� 
	currentFile=fopen(ofn.lpstrFile,"w+");     //��ʱofn����ֵ 
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
	���Ϊ 
	�����ʱ�ļ�������ʱд�ģ���ͬʱ���浱ʱ�ļ� 
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
    ZeroMemory(&ofn2, sizeof(ofn2));        //���ƵĲ��� 
    ofn2.lStructSize = sizeof(ofn2);
    ofn2.hwndOwner = NULL;
    ofn2.lpstrFile = szFile2;
    ofn2.lpstrFile[0] = '\0';
    ofn2.nMaxFile = sizeof(szFile2);
    ofn2.lpstrFilter = "�ı��ļ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0";
    ofn2.nFilterIndex = 1;
    ofn2.lpstrInitialDir = NULL;
    ofn2.Flags = OFN_PATHMUSTEXIST  | OFN_OVERWRITEPROMPT;
    if (GetSaveFileNameA(&ofn2)){
		//���������ļ������û��.txt����
		if (strlen(ofn2.lpstrFileTitle) < 4 || strcmp(ofn2.lpstrFileTitle + strlen(ofn2.lpstrFileTitle) - 4, ".txt") != 0) {
			strcat(ofn2.lpstrFileTitle, ".txt");
			strcat(ofn2.lpstrFile, ".txt");
		}
		anotherFile=fopen(ofn2.lpstrFile,"w+");
		if(isCreated){                   //�����ʱ�ļ�������ʱд�ģ���ͬʱ���浱ʱ�ļ� 
			currentFile=fopen(ofn.lpstrFile,"w+");     //��ʱofn����ֵ 
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
					fputc(rowcontent[j],currentFile);  //���Ϊ��ǰ�ļ�Ҳ����
				}
			}
		}
		fclose(anotherFile);            
    }
	else{
	     //MessageBox(NULL, "����ʧ��", NULL, MB_OK);  //�������� 
    }
	isProcessFile = 0;
}

/*
    ��ȡ��ǰ�ı���״̬
    �Ƿ񱣴� 
    �մ�����Ϊ�ѱ���
*/
int getSaveState() {
	return isSaved;
}

/*
    ���ñ���״̬
*/
void setSaveState(int newSaveState){
    isSaved=newSaveState;
}

/*
	���ص�ǰ�ļ���(������·��)
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
    �˳�ʱ��δ�������ṩѡ�� ѡ���Ƿ񱣴���� 
*/
void exitApplication(){
	if(!isSaved){
		if (!isCreated) {
			sprintf(s, "�Ƿ�Ҫ����� �ޱ��� �ĸ��ģ�");
		} else sprintf(s, "�Ƿ�Ҫ����� %s �ĸ��ģ�", ofn.lpstrFileTitle);
		int state=MessageBoxA(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
		switch(state){
			case IDYES:             //�� 
				saveFile();
				ExitGraphics();
			case IDNO:            //�� 
				ExitGraphics();
				break;
			case IDCANCEL:           //ȡ�� 
				break;
		}
	}
	else{
		ExitGraphics();
	}
}








