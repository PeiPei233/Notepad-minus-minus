/*
    ��Ҫʵ�ֲ����滻����
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "find.h"
#include "libgraphics.h"

/*
    ���ݴ�����ַ������Ҵӵ�ǰ���λ�ÿ�ʼ����һ��ƥ����ַ���
    ���ҳɹ������ѡ��ΧΪ��һ��ƥ����ַ����������¹��λ��Ϊѡ��Χĩ�ˣ������겻�ڴ���������´���λ��
*/
int findText(char *src) {
	// printf("FIND:%s\n", src);
    RCNode startCursor = getCursorRC();  //��ȡ��ǰ���λ����Ϊ���ҿ�ʼ����
    int row=startCursor.row, column=startCursor.column;
    RCNode findCursor;
    int length = strlen(src);   //��������ַ�����

    char *ch;
	ch=getCurrentString(); //��ȡ��ǰ��Ļ��ʵ������
    char *ch_=ch;    //����ָ�봢���ַ�����ʼλ��
    int flag;
    int startW = numofFormerWords(startCursor);
    ch += startW;

    while(*ch != 0)
    {   
        // printf("FIND:%s\n", ch);
        int i;
        flag = 1;
        ///�ж��Ƿ��ǲ����ַ���
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
            // printf("FIND:%s\n", ch);
            findCursor.column = column;
            findCursor.row = row;
            setSelectStartRC(findCursor);   //����ѡ��ʼ����
            findCursor.column += length;
            setSelectEndRC(findCursor);   //����ѡ���������
            setCursorRC(findCursor);    //���ù������
            setCursorInWindow();
            return 1;          
        }else  //���ǲ���������ı���������ͬʱ�ƶ�ָ��
        {
            if(*ch != '\n')
            {
                column ++;
                ch ++;
            }else
            {   
                ch++;
                column = 1;
                row++;
            }
        }
    }
    
    //���ѡ���ı�֮������û�в��ҵ����򷵻��ı���ͷ���ң�ֱ��ѡ���ı�֮ǰ
    column = 1;
    row = 1;
    int k;
    for(k=0; k<startW; k++)
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
            // printf("FIND:%s\n", ch_);
            findCursor.column = column;
            findCursor.row = row;
            setSelectStartRC(findCursor);   //����ѡ��ʼ����
            findCursor.column += length;
            setSelectEndRC(findCursor);   //����ѡ���������
            setCursorRC(findCursor);    //���ù������
            setCursorInWindow();
            return 1;          
        }else  //���ǲ���������ı���������ͬʱ�ƶ�ָ��
        {
            if(*ch_ != '\n')
            {
                column ++;
                ch_++;
            }else
            {
                ch_++;
                row ++;
                column = 1;
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
    int end = numofFormerWords(selectEnd);
    if (end < start) {
        int t = end;
        end = start;
        start = t;
    }
    int length = end - start;

    //��ȡѡ�����ݲ�������selectStr��
    char *selectStr = (char *) malloc(sizeof(char) * (end - start + 1));
    char* str = allStr;
    // strcpy(selectStr, str + start);
    for (int i = 0; i < length; i++) {
        selectStr[i] = str[start + i];
    }
    selectStr[length+1] = '\0';
    // printf("REPLACE_CMP:%s %s\n", selectStr, src);
    //�ж��Ƿ��ǲ������ݣ�����������ѡ��    
    if(strcmp(selectStr, src))    
    {
        findText(src);
        return;
    }

    if (selectStart.row > selectEnd.row || (selectStart.row == selectEnd.row && selectStart.column > selectEnd.column)) {
        RCNode t = selectStart;
        selectStart = selectEnd;
        selectEnd = t;
    }
    
    // string s;
    // s = Concat(SubString(allStr, 0, start - 1), tar);
    // //�޸�currentString 
	// setCurrentString(Concat(s,SubString(allStr, end+1, StringLength(allStr))));
    // free(selectStr);
    printf("REPLACE:FIND!\n");
    deleteSelectString();
    setSelectStartRC(selectStart);
    setSelectEndRC(selectStart);
    setCursorRC(selectStart);
    addString(tar);

    findText(src);

}
