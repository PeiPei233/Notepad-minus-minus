/**
 * 
 * ���ļ�����ʵ�ּ�¼��redo��undo�Ĳ���ʵ��
 * 
 */ 

#include "record.h"
#include "display.h"

static linkedList *nodeHead = NULL, *nodeTail = NULL;
static linkedList *curNode = NULL;


/**
 * ��ʼ��������¼
 * ��֮ǰ�м�¼���������ݣ���Ҫȫ���������
 */ 
void initRecord()
{
    linkedList *temp;
    while(nodeHead != NULL)
    {
        temp = nodeHead->next;
        free(nodeHead->str);
        free(nodeHead);
        nodeHead = temp;
    }
    nodeHead = nodeTail = curNode = NULL;
}

/**
 * ��¼����
 * op ��������: OP_ADD ����ַ��� OP_DELETE ɾ���ַ���
 * pos ����λ��
 * str ���/ɾ�����ַ���    ���ַ��������и��ģ���copy���ٴ���
 */ 
void record(int op, RCNode pos, string str) {
    if (nodeHead == NULL)   //�ж�ͷ�ڵ��Ƿ�Ϊ��
    {
        nodeHead = (linkedList*)malloc(sizeof(linkedList));
        nodeHead->op = op;
        nodeHead->pos = pos;
        nodeHead->str = str;
        nodeTail = nodeHead;
        curNode = nodeHead;
        nodeHead->last = NULL;
        nodeHead->next = NULL;
    }else
    {
        if(curNode != nodeTail)  //���Ŀǰ��������β�ڵ㣬�ͷź������нڵ�
        {
            linkedList *temp;
            while(nodeTail != curNode)
            {
                nodeTail = nodeTail->last;
                if (nodeTail == NULL) {     //����ͷ��
                    free(nodeHead->str);
                    free(nodeHead);
                    nodeHead = NULL;
                } else {
                    free(nodeTail->next->str);
                    free(nodeTail->next);
                    nodeTail->next = NULL;
                }
            }
        }
        if (nodeHead == NULL)   //�ж�ͷ�ڵ��Ƿ�Ϊ��
        {
            nodeHead = (linkedList*)malloc(sizeof(linkedList));
            nodeHead->op = op;
            nodeHead->pos = pos;
            nodeHead->str = str;
            nodeTail = nodeHead;
            curNode = nodeHead;
            nodeHead->last = NULL;
            nodeHead->next = NULL;
        }else
        {
            linkedList *temNode;
            temNode = (linkedList*)malloc(sizeof(linkedList));  
            temNode->op = op;
            temNode->pos = pos;
            temNode->str = (string)malloc(sizeof(char) * (strlen(str) + 1));
            temNode->next = NULL;
            strcpy(temNode->str, str);
            nodeTail->next = temNode;    //����˫������
            temNode->last = nodeTail;
            nodeTail = nodeTail->next;
            curNode = nodeTail;    //��ָ��ǰ�ڵ��ָ���Ƶ�����ĩ��
        }
    }
}

/**
 * ʵ�ֳ�������
 */ 
void undo() {
    if(curNode == NULL)  //�жϵ�ǰ�ڵ��Ƿ�Ϊ�գ�Ϊ���򲻽��г�������
    {
        return;
    }else
    {
        if(curNode->op == OP_ADD)   //�������Ϊ�����ַ����������ӵĲ���ɾȥ
        {
            //��λ�ַ�������λ��
            RCNode nextPos = endPos(curNode->pos, curNode->str);
            deleteContent(curNode->pos, nextPos, 0);

            //��λ���λ����ɾ���ַ��Ŀ�ʼλ��
            setCursorRC(curNode->pos);
            setSelectStartRC(curNode->pos);
            setSelectEndRC(curNode->pos);
            setCursorInWindow();

        }else if(curNode->op == OP_DELETE)   //�������Ϊɾ���ַ�����ɾ�����ַ��ӻ�ȥ
        {
            addContentByString(curNode->pos, curNode->str, 0);
            
            //��λ���λ�������ӵ��ַ�����
            RCNode nextPos = endPos(curNode->pos, curNode->str);
            setCursorRC(nextPos);
            setSelectEndRC(nextPos);
            if (strlen(curNode->str) == 1 || (strlen(curNode->str) == 2 && curNode->str[0] & 0x80)) {   //����������ǵ��ַ��򵥸�����������ѡ
                setSelectStartRC(nextPos);
            }
            else {  //������������ַ�������ѡ������Ϊ�������ַ���
                setSelectStartRC(curNode->pos);
            }
            setCursorInWindow();
        }
        curNode = curNode->last;   //��ָ��ǰ������ָ��ǰ��һλ
    }
}

/**
 * ʵ����������
 */ 
void redo() {
    if (curNode == NULL)    //������ڵĽڵ�Ϊ�գ���Ҫָ��ͷ�ڵ�
    {
        if (nodeHead != NULL)
        {
            curNode = nodeHead;
        }else
        {
            return;
        }
    }else
    {
        if(curNode->next == NULL)   //�����һ�������ڵ�Ϊ���򲻲���
        {
            return;
        }else
        {
            curNode = curNode->next;  //ָ��ָ����һ���ڵ�
        }
    }
    if(curNode->op == OP_ADD)    //���Ϊ���Ӳ��������½��ַ����ӻ�ȥ
    {
        addContentByString(curNode->pos, curNode->str, 0);

        //��λ���λ���������ַ���������
        RCNode nextPos = endPos(curNode->pos, curNode->str);
        setCursorRC(nextPos);
        setSelectEndRC(nextPos);
        setSelectStartRC(nextPos);
        setCursorInWindow();

    }else if(curNode->op == OP_DELETE)   //���Ϊɾ��������ɾ���ַ���
    {
        //�ҵ�ɾ�����ַ����Ľ���λ��
        RCNode nextPos = endPos(curNode->pos, curNode->str);
        deleteContent(curNode->pos, nextPos, 0);
        
        //��λ���λ����ɾ���ַ����Ŀ�ʼλ��
        setCursorRC(curNode->pos);
        setSelectStartRC(curNode->pos);
        setSelectEndRC(curNode->pos);
        setCursorInWindow();
    }
}

/**
 * ���㴫���ַ��Ľ�������
 * ���뿪ʼ���������м���ַ���
 * */
RCNode endPos(RCNode startPos, string str)
{
    RCNode nextPos = startPos;
    char *p = str;
    char *pp = p;
    char *enter = p;
    int isEnter = 0;
    while(*pp)
    {
        if(*pp=='\n')
        {
        	isEnter = 1;
            enter = pp;   //��λ���з�λ��
            nextPos.row ++;
        }
        pp ++;
    }
    if(isEnter)
    {
    	nextPos.column = strlen(enter);   //���һ�е�����Ϊ���з����ַ���������
	}else
	{
		nextPos.column += strlen(enter);  //����Ϊԭ�������ַ������� 
	}
    
    return nextPos;
}