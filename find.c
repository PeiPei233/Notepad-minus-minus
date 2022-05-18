/*
    ��Ҫʵ�ֲ����滻����
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "find.h"
#include "libgraphics.h"

RCNode findStr[MAX];

/*
    ���ݴ�����ַ������Ҵӵ�ǰ���λ�ÿ�ʼ����һ��ƥ����ַ���
    ���ҳɹ������ѡ��ΧΪ��һ��ƥ����ַ����������¹��λ��Ϊѡ��Χĩ�ˣ������겻�ڴ���������´���λ��
*/
int findText(char *src) {
	printf("FIND:%s\n", src);
    RCNode startCursor = getCursorRC();  //��ȡ��ǰ���λ����Ϊ���ҿ�ʼ����
    int row=startCursor.row, column=startCursor.column;
    RCNode findCursor;
    int length = strlen(src);   //��������ַ�����
    char *ch = (char*)malloc(sizeof(char)*MAX);
    strcpy(ch, getCurrentString());   //��ȡ��ǰ��Ļ��ʵ������
    char *ch_=ch;   //����ָ�봢���ַ�����ʼλ��
    int flag;
    int startW = numofFormerWords(startCursor);
    ch += startW;
    while(ch != NULL)
    {
        int i;
        flag = 1;
        //�ж��Ƿ��ǲ����ַ���
        for(i=0; i<length; i++)
        {
            if(*(ch+i) != *(src+i))
            {
                flag = 0;
                break;
            }
        }
        if(flag)   //����ҵ�����λ�����ַ�
        {
            findCursor.column = column;
            findCursor.row = row;
            if(!cursorInWindow(findCursor))  //������ڴ��������������λ��
            {
                setCursorInWindow();
            }
            setSelectStartRC(findCursor);   //����ѡ��ʼ����
            findCursor.row += length;
            setSelectEndRC(findCursor);   //����ѡ���������
            setCursorRC(findCursor);    //���ù������
            return 1;          
        }else  //���ǲ���������ı���������ͬʱ�ƶ�ָ��
        {
            ch ++;
            if(*ch != '\n')
            {
                row ++;
            }else
            {
                column ++;
            }
        }
    }
    
    //���ѡ���ı�֮������û�в��ҵ����򷵻��ı���ͷ���ң�ֱ��ѡ���ı�֮ǰ
    column = 0;
    row = 0;
    int k;
    for(k=0; k<startW-length; k++)
    {
        int i;
        flag = 1;
        //�ж��Ƿ��ǲ����ַ���
        for(i=0; i<length; i++)
        {
            if(*(ch_+i) != *(src+i))
            {
                flag = 0;
                break;
            }
        }
        if(flag)   //����ҵ�����λ�����ַ�
        {
            findCursor.column = column;
            findCursor.row = row;
            if(!cursorInWindow(findCursor))  //������ڴ��������������λ��
            {
                setCursorInWindow();
            }
            setSelectStartRC(findCursor);   //����ѡ��ʼ����
            findCursor.row += length;
            setSelectEndRC(findCursor);   //����ѡ���������
            setCursorRC(findCursor);    //���ù������
            return 1;          
        }else  //���ǲ���������ı���������ͬʱ�ƶ�ָ��
        {
            ch_ ++;
            if(*ch_ != '\n')
            {
                row ++;
            }else
            {
                column ++;
            }
        }
    }
    //�����������ʾ�ı���Ȼû���ҵ��������ݣ����÷���0
    return 0;
}


/*
    ���ݴ���������ַ������������srcΪԭ�ȵ��ַ���, tarΪ�滻�ɵ��ַ����������滻����
    �滻��ɺ�ˢ����ʾ��������ѡ��ΧΪ�滻����ַ������Ҹ��¹��λ��Ϊѡ��λ��ĩ��
    �����ǰѡ�е��ַ�������src�����Ȳ��ҹ������һ�����ϵ�src��ѡ�У�����ִ��findText(src)
    ���������ǰѡ�е��ַ�������src�����滻Ϊtar����ѡ��Χ��Ϊ�滻����ַ��������¹��λ��Ϊѡ��λ��ĩ����
*/
void replaceText(char *src, char *tar) {
    char *allStr = getCurrentString();    //��ȡ�����ı�
    RCNode selectStart, selectEnd;
    selectStart = getSelectStartRC();
    selectEnd = getSelectEndRC();
    int start = numofFormerWords(selectStart);    //��ȡѡ��Χ����ʼ����ֹλ��
    int end = numofFormerWords(selectEnd) - 1;
    int length = end - start;

    //��ȡѡ�����ݲ�������selectStr��
    char selectStr[MAX];
    char* str = allStr;
    str += start;
    strcpy(selectStr, str);
    selectStr[length+1] = '\0';

    //�ж��Ƿ��ǲ������ݣ�����������ѡ��    
    if(strcmp(selectStr, src))    
    {
        findText(src);
        replaceText(src, tar);
    }
    
    string s;
    s = Concat(SubString(allStr, 0, start - 1), tar);
    //�޸�currentString 
	setCurrentString(Concat(s,SubString(allStr, end+1, StringLength(allStr))));
}
