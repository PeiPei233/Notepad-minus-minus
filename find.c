/*
    主要实现查找替换功能
*/
#include "global.h"
#include <stdio.h>
#include "find.h"
#include <string.h>
#define CACHE1 
#define CACHE2 
FILE *fp;FILE *fp2;
/*
    根据传入的字符串查找从当前光标位置开始的下一个匹配的字符串
    查找成功后更新选择范围为下一个匹配的字符串，并更新光标位置为选择范围末端，如果光标不在窗口内则更新窗口位置
*/
RCNode findText(char *src) {
    RCNode cursor = getCursorRC();
    RCNode mycursor;
    char ch;int row=0,column=0;
    if((fp=fopen(CACHE2,"r"))==NULL)
    	exit(0);
    while(row<cursor.row||column<cursor.column){         //前提光标不要超出文本范围 
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
				return {0,0};                       //留下空间，没找到或许输出提示？ 
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
    根据传入的两个字符串（这里假设src为原先的字符串, tar为替换成的字符串）进行替换操作
    替换完成后刷新显示，并更新选择范围为替换后的字符串，且更新光标位置为选择位置末端
    如果当前选中的字符串不是src，则先查找光标后的下一个符合的src并选中，即先执行findText(src)
    否则如果当前选中的字符串就是src，则替换为tar并将选择范围改为替换后的字符串并更新光标位置为选择位置末端吗
*/
void replaceText(char *src, char *tar) {
	updateCache();
	fp=fopen(CACHE1,"r");fp2=fopen(CACH2,"w");
	setCursorRC(getSelectStartRC());     //查找选择范围后的该字符串位置 
	RCNode mynode=findText(src);
	if(nullStruct(mynode))
		return;               // 需要输出什么吗？ 
	int row=0,column=0;
	char ch;
	while(row<mynode.row||column<(mynode.column-strlen(src))){         //前提光标不要超出文本范围 
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
	display();            //也许需要getcurrentstring? 
}














