/* ����Ҫȷ��ϵͳ�ļ�λ�ã���ʼ��CACHE1��CACHE2;(������������½�һ��cache�ļ���) 
   ��δ򿪺���Ҫ���̳�ʼ�����棬������initCache()
   �ٴ�winrow ��wincolumn��Ҫ����global�ĺ�������֪զ���Ҳ������µ� 
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
static FILE *currentFile;  //��ǰ�ļ� 
static FILE *cacheFile1;    //�����ļ� 
static FILE *cacheFile2;
OPENFILENAME ofn;
// a another memory buffer to contain the file name
char szFile[100];
static int isSaved;    //�Ƿ񱣴� 
static int isCreated;  //�Ƿ��Ѿ�ȷ������λ�� 
static int winrow; 
static int wincolumn;     //��ط�Ӧ����Ҫ�����ط������ݣ���ʾһ�������м��м��� 
/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
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
	    MessageBox(NULL, "open failed", NULL, MB_OK);  //�򿪴��� 
	fileWrite(currentFile,cacheFile1);            //������������ 
	fileWrite(currentFile,cacheFile2);
	isSaved=0;
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
    ��ʼ�������ļ�
*/
void initCache() {
	cacheFile1=fopen(CACHE1,"w+");
	cacheFile2=fopen(CACHE2,"w+");
	isCreated=0;
}

/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
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
			MessageBox(NULL, "create failed", NULL, MB_OK);  //������� 
		
	}
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
    �Զ��崫��Ĳ��� �細�����Ͻǵ��������� ���ݴ�������ӻ����ļ��и���currentString       ���ֵܣ�����currentstringҲ����ȫ�ֱ�����������ɶ�� 
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    ���ݴ�����ַ�����currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
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
	fseek(cacheFile1,0L,0);             //�����Ѿ���ɻ���ĸ��� 
	if(cursor.column==windowCursor.column+wincolumn-1)
		windowCursor.column++;
	setWindowCurrentRC(windowCursor) ;	           //�����ƶ� 
	
	cursor.column++;
	setCursorRC(cursor);       
	getCurrentString();     //�������ڵ��ַ���     
}

/*
    ���ݴ�����ַ�������currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void addString(char *src) {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 ���� 
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
	fseek(cacheFile1,0L,0);             //�����Ѿ���ɻ���ĸ��� 
	int x=cursor.column+strlen(src)-windowCursor.column-wincolumn+1;
	if(x>0)                                         //x>0 ��ʾ����Ӧ���ƶ������� ��ʵ���ǰ��ַ������ȼ��ϣ�����Ҫ���� 
		windowCursor.column+=x;
	setWindowCurrentRC(windowCursor);
	cursor.column+=strlen(src);
	setCursorRC(cursor); 
	getCurrentString();            //������ʾ 
}

/*
    ���ݵ�ǰ���λ�ã�ɾ�����ǰ��һ���ַ�����currentString�뻺���ļ��У�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
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
	fgetc(cacheFile1);           // ������cache2һģһ����cache1��ɲ�����ֻ�轫cache1���һλ�� 
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
    ���ݵ�ǰѡ�еķ�Χ��ɾ��ѡ�е��ַ�������currentString��cacheFile�У�
    �����¹���Լ�ѡ�з�Χ���������꣨���б�ҪҲ���´������Ͻ�λ�ã�
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
	fgetc(cacheFile1);     //������Χ�Ǹ��ַ�ҲӦɾȥ 
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









