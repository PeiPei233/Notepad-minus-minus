/*
    ��Ҫʵ���ļ���������ع���
*/
/*
 ÿ���ļ��༭�ǻḽ������cache�ļ����ֱ�洢��һ��������͵�ǰ������ 
 ��ǰ�ַ������������� 
*/
#include <stdio.h>
#include "file.h"

#include <string.h>

#define _N 1000010
#define CACHE1
#define CACHE2
#define FILELOCATION
static char currentString[_N];
static FILE *currentFile;  //��ǰ�򿪵��ļ�
static FILE *cacheFile1;    //�����ļ�
static FILE *cacheFile2;
static int isSaved;     //�Ƿ񱣴�
static int winrow= GetWindowHeight() / getFontHeight();
static int wincolumn= GetWindowWidth() / 
/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
*/
void openFile() {
	currentFile=fopen(FILELOCATION,"r+");
	fileWrite(currentFile,cacheFile1);            //Ҫ�󻺴��ļ����ȴ� 
	fileWrite(currentFile,cacheFile2);
	isSaved=0;
}

/*
    �½�һ���ļ�
    �����½����ļ����ڱ���ʱҪѡ����λ��
*/
void createFile() {
    currentFile=fopen(,"w+");
    isSaved=0;
}

void fileWrite(FILE* fp1,FILE* fp2){
	while(!feof(fp1)){
		ch=fgetc(fp1);
		fputc(ch,fp2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);	
}

/*
    ��ʼ�������ļ�
*/
void initCache() {
	
}

/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
*/
void saveFile() {
	isSaved=1;
	fileWrite(cacheFile2,currentFile);
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
	RCNode windowRC=getWindowCurrentRC();
	int row=0,column=0;
	char ch;int i=0;
    while(row<windowRC.row||column<windowRC.column){        //�ƶ������Ͻ� 
    	ch=fgetc(cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	row=1,column=1;
	while(!feof(cacheFile2) && (row<=winrow)){
		ch=fgetc(cacheFile2);
		if(ch=='\n'||column==windowRC.column+wincolumn){    //���л��߳�����ʾ��Χ 
			row++;column=0;
			while(column<windowRC.column){
				fgetc(cacheFile2);
				column++;
			}
		}
		else{
			column++;
		}
		
		currentString[i++]=ch;
	}
	fseek(cacheFile2,0L,0);          //�ص��ײ� 
	currentString[i]='\0';      //���и����⣬��������黹�ܱ����� 
	return currentString;
}

/*
    �Զ��崫��Ĳ��� �細�����Ͻǵ��������� ���ݴ�������ӻ����ļ��и���currentString      ��Ҿ���ûɶ��Ҫ�� 
*/
void updateCurrentString() {
    RCNode windowCurrent = getWindowCurrentRC();
}

/*
    ���ݴ�����ַ�����currentString�뻺���ļ�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�          Ӧ������֮���һ���� 
*/
void addChar(char ch) {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 ���� 
    RCNode cursor = getCursorRC();
    RCNode windowCursor=getWindowCurrentRC();
    int row=0,column=0;char chx;
    while(row<cursor.row||column<cursor.column){        
    	chx=fgetc(cacheFile2);
    	if(chx=='\n')
    		row++;
    	else
    		column++;
	} 
	fputc(ch,cacheFile2);
	if(cursor.column==windowCursor.column+wincolumn-1)
		windowCursor.column++;
	setWindowCurrentRC(windowCursor) ;	           //���з��ƺ����ÿ��� 
	
	cursor.column++;
	setCursorRC(cursor);       
	fseek(cacheFile2,0L,0);
	getCurrentString();          // Ҳ���ڵ��õĵط��ø���һЩ�� 
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
	int x=cursor.column+strlen(src)-windowCursor.column-wincolumn+1;
	if(x>0)                                         //x>0 ������Ҫ�ƶ����� 
		windowCursor.column+=x;
	setWindowCurrentRC(windowCursor);
	fseek(cacheFile2,0L,0);
	getCurrentString();
}

/*
    ���ݵ�ǰ���λ�ã�ɾ�����ǰ��һ���ַ�����currentString�뻺���ļ��У�
    �����¹��λ�ã����б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteChar() {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 ���� 
    RCNode cursor = getCursorRC();
    fclose(cacheFile2);
    cacheFile2=fopen(CACHE2,"w");
    int row=0,column=0;
    while(row<cursor.row||column<cursor.column){        
    	ch=fgetc(cacheFile1);
    	fputc(ch,cacheFile2);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
	fgetc(cacheFile1);
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		fputc(ch,cacheFile2);
	}
	fseek(cacheFile2,0L,0);
	fseek(cacheFile1,0L,0);
	getCurrentString();
}

/*
    ���ݵ�ǰѡ�еķ�Χ��ɾ��ѡ�е��ַ�������currentString��cacheFile�У�
    �����¹���Լ�ѡ�з�Χ���������꣨���б�ҪҲ���´������Ͻ�λ�ã�
*/
void deleteSelectString() {
	fileWrite(cacheFile1,cacheFile2);     //cacheFile1 ���� 
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();                 //��������Ӧ����ͬһ�е� 
    fclose(cacheFile2);
    cacheFile2=fopen(CACHE2,"w");
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
	for( i=0;i<=endSelect.column-startSelect.column;i++)
		fgetc(cacheFile1);
	while(!feof(cacheFile1)){
		ch=fgetc(cacheFile1);
		fputc(ch,cacheFile2);
	}
	setCursorRC(startSelect);
	setSelectEndRC(startSelect); //��ʼλ�ú��յ�λ����ͬ 
	fseek(cacheFile1,0L,0);
	fseek(cacheFile2,0L,0);
	getCurrentString();
}









