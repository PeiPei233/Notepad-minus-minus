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

static FILE *currentFile;  //��ǰ�ļ� 
OPENFILENAMEA ofn;
// a another memory buffer to contain the file name
static char szFile[512];       //�洢�ļ��� 
static char szFileTitle[512];
static int isSaved;    //�Ƿ񱣴�         
static int isCreated;     //�Ƿ񱻴��� 
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
    }
	else{
	    // MessageBox(NULL, "��ʧ��", NULL, MB_OK);  //�򿪴���
        return;
    } 
    int state;
    if(!(getTotalRow()==1&&getRowLength(1)==0)){            //��δ�������ҵ�ǰ��ʾ�ַ�����Ϊ0ʱ�赯��������ʾ
        state = MessageBoxA(NULL,"�򿪺�����ǰ�����ݽ������ǣ�ȷ������","Notepad--",MB_OKCANCEL | MB_TASKMODAL);
        if(state==IDOK)    //ok
            ;
        else if(state==IDCANCEL){ //cancel
        	fclose(currentFile);
            return;
        }
    }
    initStorage();      //����ԭ�ȵ�����
    initRecord(); 
    unsigned int degree=0;
	char ch;
	RCNode cursor = {1, 1};
	while(!feof(currentFile)){
		ch=fgetc(currentFile);
		if(ch!=EOF)
			addContent(BY_CHAR,cursor,&ch,0);
		if (ch=='\n'){
			cursor.row++;
			cursor.column=1;
		}
		else
			cursor.column++;
		degree++;
		if(degree>=1<<31){                     //������2G,���޷���
			MessageBoxA(NULL,"�ļ������볢���� VSCode ��Ӧ�ô� :)",NULL,MB_OKCANCEL | MB_TASKMODAL);
			initStorage();
			return;
		}
	}
    fclose(currentFile);
	isSaved=1;
	isCreated=1;
}

/*
    �½�һ���ļ�
    �����½����ļ����ڱ���ʱҪѡ����λ��
*/
void createFile() {
	if(getTotalRow()==1&&*getRowContent(1)==0){      //���������ȫ�հ� ,���赯������ 
		;
	}
	else{
		int state  =MessageBoxA(NULL,"ȷ��Ҫ���ǵ�ǰ�ļ���","Notepad--",MB_OKCANCEL | MB_TASKMODAL);
		if(state==IDOK){
			;             //������һ�� 
		}
		else if(state==IDCANCEL){
			return;
		}
	}
	// create a file name
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "�ı��ļ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.lpstrFileTitle[0] = '\0';
    ofn.nMaxFileTitle = sizeof(szFileTitle);
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST;
    if (GetSaveFileNameA(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"w+");
        fclose(currentFile);
    }
	else{
	    // MessageBox(NULL, "����ʧ��", NULL, MB_OK);  //�������� 
    }
    isSaved=1;
    isCreated=1;
}

/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
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
	    ofn.lpstrFilter = "�ı��ĵ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0";
	    ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = szFileTitle;
   		ofn.lpstrFileTitle[0] = '\0';
   	 	ofn.nMaxFileTitle = sizeof(szFileTitle);
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	    if (GetSaveFileNameA(&ofn)){
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

}

/*
	���Ϊ 
	�����ʱ�ļ�������ʱд�ģ���ͬʱ���浱ʱ�ļ� 
*/
void saveAsFile(){
	if (!isCreated) {
		saveFile();
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

static char s[1010];

/*
    �˳�ʱ��δ�������ṩѡ�� ѡ���Ƿ񱣴���� 
*/
void exitApplication(){
	if(!isSaved){
		if (ofn.lpstrFile == NULL) {
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








