/*
    ��Ҫʵ�ֲ����滻����
*/
#include "global.h"
#include <stdio.h>
#include "find.h"
#include <string.h>
#define CACHE1 
#define CACHE2 
FILE *fp;FILE *fp2;
/*
    ���ݴ�����ַ������Ҵӵ�ǰ���λ�ÿ�ʼ����һ��ƥ����ַ���
    ���ҳɹ������ѡ��ΧΪ��һ��ƥ����ַ����������¹��λ��Ϊѡ��Χĩ�ˣ������겻�ڴ���������´���λ��
*/
RCNode findText(char *src) {
    RCNode cursor = getCursorRC();
    RCNode mycursor;
    char ch;int row=0,column=0;
    if((fp=fopen(CACHE2,"r"))==NULL)
    	exit(0);
    while(row<cursor.row||column<cursor.column){         //ǰ���겻Ҫ�����ı���Χ 
    	ch=fgetc(fp);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
	} 
    while(!feof(fp)){
    	int flag=1;
    	ch=fgetc(fp);
    	if(ch=='\n')
    		row++;
    	else
    		column++;
    	int back=0;
    	if(*src!=ch);
		else{
			for(int i=1;i<strlen(src);i++){
				if(*(src+i)!=fgetc(fp)){
					flag=0;
					break; 
				}
				else{
					back++;
				}
			}
			fseek(fp,-(back+1),1);
			if(flag==1){
				mycursor.row=row;mycursor.column=column;
				setSelectStartRC(mycursor);
				mycursor.column+=strlen(src);
				setSelectEndRC(mycursor);
				if(!cursorInWindow())
					setCursorInWindow();
				setCurSorRC(mycursor);
				fclose(fp);
				display();
				return mycursor;
			}
			else{
				fclose(fp);
				display();
				return {0,0};                       //���¿ռ䣬û�ҵ����������ʾ�� 
			}
		}
	}
}
int nullStruct(RCNode temp){
	if(temp.column==0&&temp.row==0)
		return 1;
	else
		return 0;
}
/*
    ���ݴ���������ַ������������srcΪԭ�ȵ��ַ���, tarΪ�滻�ɵ��ַ����������滻����
    �滻��ɺ�ˢ����ʾ��������ѡ��ΧΪ�滻����ַ������Ҹ��¹��λ��Ϊѡ��λ��ĩ��
    �����ǰѡ�е��ַ�������src�����Ȳ��ҹ������һ�����ϵ�src��ѡ�У�����ִ��findText(src)
    ���������ǰѡ�е��ַ�������src�����滻Ϊtar����ѡ��Χ��Ϊ�滻����ַ��������¹��λ��Ϊѡ��λ��ĩ����
*/
void replaceText(char *src, char *tar) {
	updateCache();
	fp=fopen(CACHE1,"r");fp2=fopen(CACH2,"w");
	setCursorRC(getSelectStartRC());     //����ѡ��Χ��ĸ��ַ���λ�� 
	RCNode mynode=findText(src);
	if(nullStruct(mynode))
		return;               // ��Ҫ���ʲô�� 
	int row=0,column=0;
	char ch;
	while(row<mynode.row||column<(mynode.column-strlen(src))){         //ǰ���겻Ҫ�����ı���Χ 
	    	ch=fgetc(fp);
	    	fputc(ch,fp2); 
	    	if(ch=='\n')
	    		row++;
	    	else
	    		column++;
	}
	for(int i=0;i<strlen(src);i++){
		fgetc(fp);
	}
	for(int i=0;i<strlen(tar);i++)
		fputc(*(tar+i),fp2);
		
	while(!feof(fp)){
		ch=fgetc(fp);
		fputc(ch,fp2);
	}
	fclose(fp);fclose(fp2);
	display();            //Ҳ����Ҫgetcurrentstring? 
}














