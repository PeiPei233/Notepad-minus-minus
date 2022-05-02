#include "global.h"
#include "libgraphics.h"

static RCNode windowCurrent; //�������Ͻǵ���������
static RCNode cursor;  //������ڵ���������
static RCNode startSelect, endSelect;  //ѡ��Χ�����/�յ�����λ��
int bigin, end;

/*
    �û���ȡĿǰ�������Ͻǵ�����λ��
*/
RCNode getWindowCurrentRC() 
{
    return windowCurrent;
}

/*
    �û�����Ŀǰ�������Ͻǵ�����λ��
*/
void setWindowCurrentRC(RCNode newWindowCurrent) 
{
    windowCurrent = newWindowCurrent;
}

/*
    ���ݴ���Ĳ������ù��λ��
*/
void setCursorRC(RCNode newCursor) 
{
    cursor = newCursor;
}

/*
    ��ȡ���λ��
*/
RCNode getCursorRC() 
{
    return cursor;
}

/*
    ��ȡѡ��Χ��ʼλ��
*/
RCNode getSelectStartRC() 
{
    return startSelect;
}

/*
    ��ȡѡ��Χ��ֹλ��
*/
RCNode getSelectEndRC() 
{
    return endSelect;
}

/*
    ����ѡ��Χ��ʼλ��
*/
void setSelectStartRC(RCNode newStartSelect)
{
    startSelect = newStartSelect;
}

/*
    ����ѡ��Χ��ֹλ��
*/
void setSelectEndRC(RCNode newEndSelect)
{
    endSelect = newEndSelect;
}

/*
    �жϹ��λ���Ƿ��ڴ�����
*/
int cursorInWindow(RCNode curCursor) 
{
    
}

/*
    ���Ĺ��λ��ʹ���ܹ�չʾ�ڴ�����
    �������Ѿ��ڴ������򲻵���
*/
void setCursorInWindow() 
{
    
}

/*
    ���ݴ������������������ַ������е�λ�ã��ڼ�����
*/

int numofFormerWords(RCNode curPosition)
{
    char word_[max];
    char *word = word_;
	strcpy(word_, allText());
    int num=0;  //ָ���ַ�������λ��
    int searchColumn=1;   //����ָ��ָ���ı��ļ������ַ���λ�ڵ���������
    int searchRow=0;
    int enterRow=0;   //���廻�з����ڵĸ��е�λ��
    int isEnter=0;   //�ж��Ƿ���
    while(*word != '\0')
    {
        //������ͼ�ν���ÿ������ַ��������������з�ʱ����
        if(searchRow == 1<<30)  //maxnΪһ���ܴﵽ������ַ���
        {
            searchColumn ++;
            searchRow = 0;
            isEnter = 0;
        }else if(*word == '\n')
        {
            enterRow = searchRow;
            searchColumn ++;
            searchRow = 0;
            isEnter = 1;
        }
        //������ȣ��ҵ�ָ��λ��
        if(searchColumn == curPosition.column && searchRow == curPosition.row)
        {
        	num ++;
            break;
        //�����λ�ڻ��������еĿհ״�����λ������ĩβ
        }else if(isEnter == 1)
        {
	    	if((searchColumn-1) == curPosition.column && curPosition.row >= enterRow)
	        {
	            break;
	        }
	        isEnter = 0;
	        searchRow ++;
			word ++;
			num ++;
        //�������ָ����ƣ���������
		}else
        {
        	word ++;
        	searchRow ++;
        	num ++;
		}
    }
    return num;
}

/*
    �������ı����ݷ���һ�����鵱����������
*/
char* allText()
{
    FILE *fp;
    char colunmStr[max];    //������ʱ����ÿ���ı��ַ�������
    char allText[max];  //���崢�������ı�������
    strcpy(allText, "");
    if(fp = fopen("currentFile.txt", "r"))
    {
        //���н��ж�ȡ������
        while(fgets(colunmStr, max, fp))
        {
            strcat(allText, colunmStr);
            strcpy(colunmStr,"");
        }
        fclose(fp);
        return allText;
    }else
    {
        printf("�򿪴���");
        return NULL;
    }
}