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

static FILE *currentFile;  //��ǰ�ļ� 
OPENFILENAME ofn;
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
    if (GetOpenFileName(&ofn)){
		currentFile=fopen(ofn.lpstrFile,"r+");
    }
	else{
	    // MessageBox(NULL, "��ʧ��", NULL, MB_OK);  //�򿪴���
        return;
    } 
    int state;
    if(!(getTotalRow()==1&&getRowLength(1)==0)){            //��δ�������ҵ�ǰ��ʾ�ַ�����Ϊ0ʱ�赯��������ʾ
        state = MessageBox(NULL,"�򿪺�����ǰ�����ݽ������ǣ�ȷ������","Notepad--",MB_OKCANCEL | MB_TASKMODAL);
        if(state==IDOK)    //ok
            ;
        else if(state==IDCANCEL){ //cancel
        	fclose(currentFile);
            return;
        }
    }
    initStorage();      //����ԭ�ȵ�����
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
		if(degree>=1<<31){                     //������2G,���޷���
			MessageBox(NULL,"�ļ������볢���� VSCode ��Ӧ�ô� :)",NULL,MB_OK | MB_TASKMODAL);
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
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetSaveFileName(&ofn)){
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
	    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	    if (GetSaveFileName(&ofn)){
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
		int state=MessageBox(NULL,s,"Notepad--",MB_YESNOCANCEL | MB_ICONWARNING | MB_TASKMODAL);
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








