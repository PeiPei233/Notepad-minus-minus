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
#define _N 10000
static char *currentString;
static FILE *currentFile;  //��ǰ�ļ� 
OPENFILENAME ofn;
// a another memory buffer to contain the file name
static char szFile[100];       //�洢�ļ��� 
static int isSaved;    //�Ƿ񱣴�         
static int isCreated;     //�Ƿ񱻴��� 
/*
	��ʼ���ļ����� , һ��ʾͼ�ν����Ҫ���� 
*/
void initFileConfig(){
	isSaved=1;
	isCreated=0;
	initStorage();
}
/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
*/
void openFile() {
    int state;
    if(!isCreated&&!(getTotalRow()==1&&getRowLength(1)==0)){            //��δ�������ҵ�ǰ��ʾ�ַ�����Ϊ0ʱ�赯��������ʾ
        state = MessageBox(NULL,"�򿪺�����ǰ�����ݽ������ǣ�ȷ������",NULL,MB_OKCANCEL);
        if(state==1)    //ok
            ;
        else if(state==2) //cancel
            return;
    }
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
    if (GetOpenFileName(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"r+");
    }
	else{
	    MessageBox(NULL, "��ʧ��", NULL, MB_OK);  //�򿪴���
        return;
    } 
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
			degree*=2;
			temp=currentString;
			currentString=(char *)malloc(_N*degree);
			for(j=0;j<i;j++){
				currentString[j]=temp[j];
			}
			free(temp);
		}
	}
    if(degree>=2e9){                     //������2G,���޷���
        MessageBox(NULL,"�ļ�����",NULL,MB_OK);
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
    if (GetSaveFileName(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"w+");
        fclose(currentFile);
    }
	else{
	    MessageBox(NULL, "����ʧ��", NULL, MB_OK);  //�������� 
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
	    ofn.lpstrFile[0] = '\0';
	    ofn.nMaxFile = sizeof(szFile);
	    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	    ofn.nFilterIndex = 1;
	    ofn.lpstrFileTitle = NULL;
	    ofn.nMaxFileTitle = 0;
	    ofn.lpstrInitialDir = NULL;
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	    if (GetSaveFileName(&ofn)){
			if(currentFile=fopen(ofn.lpstrFile,"w+")){
				isCreated = 1;
                fclose(currentFile);
            }
		}
		else{
			MessageBox(NULL, "����ʧ��", NULL, MB_OK);  //�������� 	
        }
	}
	if(!isSaved){         //���δ���棬д��currentFile�� 
		currentFile=fopen(ofn.lpstrFile,"w+");     //��ʱofn����ֵ 
		int i,row=getTotalRow();
        for(i=1;i<=row;i++){
            char *rowcontent=getRowContent(i);
            fputs(rowcontent,currentFile);
        }
		isSaved=1;
		fclose(currentFile);      //Ҫ���� 
	}
	else  ;                //Ҳ����callback�����ã� 
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
void saveSetState(int newSaveState){
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
    �˳�ʱ��δ�������ṩѡ�� ѡ���Ƿ�Ҫ�˳� 
    ��ȷ���˳��򷵻�1
*/
int certainToExit(){
    if(!isSaved){
        int a=MessageBox(NULL,"���Ĺ���δ���棬�Ƿ��˳�",NULL,MB_OKCANCEL);
        if(a==1)       //OK
            return 1;
        else if(a==2){       //Cancel
            return 0;
        }
    }
    else
        return 1;
}









