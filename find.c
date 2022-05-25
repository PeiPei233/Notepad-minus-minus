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
#include "display.h"
#include "record.h"

static int restart=0; //��¼�Ƿ�����ɨ�� 
static int recordAll=0; //�жϲ����Ƿ�ΪreplaceAll 
static int recordID;
/**
 * ���ݴ�����ַ����ӹ��λ�ã�ѡ��Χǰ��������һ��ƥ����ַ���
 * ���ҳɹ������ѡ��ΧΪ��һ��ƥ����ַ����������¹��λ��Ϊѡ��Χĩ�ˣ������겻�ڴ���������´���λ��
 */ 
int findLastText(const char *src) {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    RCNode cursor = getCursorRC();
    int len = strlen(src);      //�����ַ����ĳ���
    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }
    cursor = startSelect;   //��ѡ��Χ�Ŀ�ͷ��ʼ���ϲ���
    int row = cursor.row, col = cursor.column - 1;  //�ӹ��ǰ��ʼ��
    string s = getRowContent(row);
    int find = 0;   //�Ƿ���ҵ�
    for (row = cursor.row; row >= 1; row--) {
        if (row == cursor.row) col = cursor.column - 1;
        else col = getRowLength(row) - len + 1;
        string s = getRowContent(row);
        while (col >= 1) {
            int flag = 1;   //�����ж��ַ����Ƿ����
            for (int j = col - 1; j < col - 1 + len; j++) {   //col�±��1��ʼ��Ҫ-1
                if (s[j] != src[j + 1 - col]) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {     
                find = 1;
                break;
            }
            col--;
        }
        if (find) {
            break;
        }
    }
    if (!find) {    //�����δ�ҵ���������½�β����ǰ��
        for (row = getTotalRow(); row >= cursor.row; row--) {
            col = getRowLength(row) - len + 1;
            string s = getRowContent(row);
            while (col >= 1) {
                int flag = 1;   //�����ж��ַ����Ƿ����
                for (int j = col - 1; j < col - 1 + len; j++) {   //col�±��1��ʼ��Ҫ-1
                    if (s[j] != src[j + 1 - col]) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) {
                    find = 1;
                    break;
                }
                col--;
            }
            if (find) {
                break;
            }
        }
        if (!find) {    //�������û�ҵ�
            return 0;
        }
    }
    if (!find) return 0;
    //�ɹ��ҵ�������ѡ��Χ
    setSelectStartRC((RCNode) {row, col});
    setSelectEndRC((RCNode) {row, col + len});
    setCursorRC((RCNode) {row, col + len});
    setCursorInWindow();
    return 1;
}

/*
    ���ݴ�����ַ������Ҵӵ�ǰ���λ�ÿ�ʼ����һ��ƥ����ַ���
    ���ҳɹ������ѡ��ΧΪ��һ��ƥ����ַ����������¹��λ��Ϊѡ��Χĩ�ˣ������겻�ڴ���������´���λ��
*/
int findNextText(const char *src) {
	// printf("FIND:%s\n", src);
    RCNode startCursor = getSelectEndRC();  //��ѡ��Χ�յ���Ϊ���ҿ�ʼ����
    int row=startCursor.row, column=startCursor.column;
    RCNode findCursor;
    int length = strlen(src);   //��������ַ�����

    char *str;
	str = getRowContent(row); //��ȡ��ǰ����ʾ������
    int flag;
    str += column-1;
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
                printf("FIND:\n");
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
        column=1;      //���У����ʼ��column 
        str = getRowContent(row); //��ȡ��ǰ����ʾ������
    }
    //��δ�ҵ��򷵻ؿ�ͷ��������
    row=1;column=1; 
    str=getRowContent(row);
    restart++;                     //��¼�����Ĵ�����ΪreplaceAll��׼�� 
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
        column=1;
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
int replaceText(const char *src, const char *tar) {
    RCNode selectStart, selectEnd;
    selectStart = getSelectStartRC();
    selectEnd = getSelectEndRC();
	int findflag=1;
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
	if(!selectStr){
		if(!findNextText(src)){
			findflag=0;
		}
	}   
    else if(strcmp(selectStr, src))    
    {
        if(!findNextText(src)){
        	findflag=0;
		}
    }
    if(findflag){             //�����ҵ�������滻 
	    printf("REPLACE:FIND!\n");
	    deleteContent(selectStart,selectEnd,1);     //ɾ��Դ�ַ��� 
	    addContent(BY_STRING,selectStart,tar,1);  //ճ��Ŀ���ַ��� 
	    
	    if(!recordAll){                               //��¼��������Ϊһ�� ��ΪreplaceAll,��ȫ��Ϊһ�� 
	    	recordID=newRecordID();
		}
	    record(OP_DELETE,selectStart,src,recordID);
	    record(OP_ADD,selectStart,tar,recordID);
	    
	    setSelectStartRC(selectStart);    
	    selectStart.column+=strlen(tar);
	    setSelectEndRC(selectStart);              //����ѡ��Χ 
	    setCursorRC(selectStart);
	    findNextText(src);
		free(selectStr);
		
	    return 1; 
	}
	else{				//�������ҵ����������0 
		return 0;
	}
}

/*
	�滻ȫ�� 
*/ 
void replaceAll(const char *src,const char *tar){
	restart=0;
	recordAll=1;
	recordID=newRecordID();
	RCNode start=getSelectStartRC();
	RCNode now=getSelectStartRC();
	while(1){
        now=getSelectStartRC();
        if(!((restart==0&&RCcompare(now,start)>=0)||(restart==1&&RCcompare(start,now)>0))){
            break;
        }
        if(!replaceText(src,tar)){
        	break;
		}
	}
	restart=0;
	recordAll=0;      //�ָ�һ��״̬ 
}
