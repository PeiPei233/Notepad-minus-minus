/*
    ��Ҫʵ�ֲ����滻����
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "global.h"
#include "find.h"
#include "libgraphics.h"
#include "storage.h"
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

    char *str;
	str = getRowContent(row); //��ȡ��ǰ����ʾ������
    int flag;
    str += column;
    int totalrow=getTotalRow();
    while(row<=totalrow){
        while(*str != 0)
        {   
            // printf("FIND:%s\n", ch);
            int i;
            flag = 1;
            //�ж��Ƿ��ǲ����ַ���
            for(i=0; i<length&&*str!=0; i++)
            {
                if(*(str+i) != *(src+i))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag&&i==length)   //����ҵ�����λ�����ַ�
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
            }else  //���ǲ����������ƶ�ָ��
            {
                    column ++;
                    str ++;
            }
        }
        row++;
        str = getRowContent(row); //��ȡ��ǰ����ʾ������
    }
    //��δ�ҵ��򷵻ؿ�ͷ��������
    row=0;
    str=getRowContent(row);
    while(row<=totalrow){
        while(*str != 0)
        {   
            // printf("FIND:%s\n", ch);
            int i;
            flag = 1;
            //�ж��Ƿ��ǲ����ַ���
            for(i=0; i<length&&*str!=0; i++)
            {
                if(*(str+i) != *(src+i))
                {
                    flag = 0;
                    break;
                }
            }
            if(flag&&i==length)   //����ҵ�����λ�����ַ�
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
            }else  //���ǲ����������ƶ�ָ��
            {
                    column ++;
                    str ++;
            }
        }
        row++;
        str = getRowContent(row); //��ȡ��ǰ����ʾ������
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
    RCNode selectStart, selectEnd;
    selectStart = getSelectStartRC();
    selectEnd = getSelectEndRC();

	//�趨Ϊstart��ǰ��end�ں� 
    if (selectStart.row > selectEnd.row || (selectStart.row == selectEnd.row && selectStart.column > selectEnd.column)) {
        RCNode t = selectStart;
        selectStart = selectEnd;
        selectEnd = t;
    }
    //��ȡѡ�����ݲ�������selectStr��
    char *selectStr = getContent(selectStart,selectEnd);
 
    // printf("REPLACE_CMP:%s %s\n", selectStr, src);
    //�ж��Ƿ��ǲ������ݣ�����������ѡ��    
    if(strcmp(selectStr, src))    
    {
        findText(src);
        return;
    }
    
    printf("REPLACE:FIND!\n");
    deleteContent(selectStart,selectEnd,1);     //ɾ��Դ�ַ��� 
    addContent(BY_STRING,selectStart,tar,1);  //ճ��Ŀ���ַ��� 
    setSelectStartRC(selectStart);
    setSelectEndRC(selectStart);              //����ѡ��Χ 
    setCursorRC(selectStart);
	free(selectStr);
    findText(src);

}
