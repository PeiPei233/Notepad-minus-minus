/**
 * 
 * ???????redo undo????
 * 
 */ 

#include "unredo.h"

//??????????????????????

/**
 * ��¼����
 * op ��������: OP_ADD ����ַ��� OP_DELETE ɾ���ַ���
 * pos ����λ��
 * str ���/ɾ�����ַ���
 */ 
void record(int op, RCNode pos, string str) {
    if (nodeHead==NULL)   //�ж�ͷ�ڵ��Ƿ�Ϊ��
    {
        nodeHead = (linkedList*)malloc(sizeof(linkedList));
        nodeHead->op = op;
        nodeHead->pos = pos;
        nodeHead->str = (string)malloc(sizeof(str));
        strcpy(nodeHead->str, str);
        nodeTail = nodeHead;
        curNode = nodeHead;
        nodeHead->last = NULL;
    }else
    {
        linkedList *temNode;
        temNode = (linkedList*)malloc(sizeof(linkedList));  
        temNode->op = op;
        temNode->pos = pos;
        temNode->str = (string)malloc(sizeof(str));
        strcpy(temNode->str, str);
        nodeTail->next = temNode;    //����˫������
        temNode->last = nodeTail;
        nodeTail = nodeTail->next;
        curNode = nodeTail;    //��ָ��ǰ�ڵ��ָ���Ƶ�����ĩ��
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

        }else if(curNode->op == OP_DELETE)   //�������Ϊɾ���ַ�����ɾ�����ַ��ӻ�ȥ
        {
            addContentByString(curNode->pos, curNode->str, 0);
            
            //��λ���λ�������ӵ��ַ�����
            RCNode nextPos = endPos(curNode->pos, curNode->str);
            setCursorRC(nextPos);
        }
        curNode = curNode->last;   //��ָ��ǰ������ָ��ǰ��һλ
    }
}

/**
 * ʵ����������
 */ 
void redo() {
    if(curNode->next == NULL)   //�����һ�������ڵ�Ϊ���򲻲���
    {
        return;
    }else
    {
        curNode = curNode->next;  //ָ��ָ����һ���ڵ�
        if(curNode->op == OP_ADD)    //���Ϊ���Ӳ��������½��ַ����ӻ�ȥ
        {
            addContentByString(curNode->pos, curNode->str, 0);

            //��λ���λ���������ַ���������
            RCNode nextPos = endPos(curNode->pos, curNode->str);
            setCursorRC(nextPos);

        }else if(curNode->op == OP_DELETE)   //���Ϊɾ��������ɾ���ַ���
        {
            //�ҵ�ɾ�����ַ����Ľ���λ��
            RCNode nextPos = endPos(curNode->pos, curNode->str);
            deleteContent(curNode->pos, nextPos, 0);
            
            //��λ���λ����ɾ���ַ����Ŀ�ʼλ��
            setCursorRC(curNode->pos);
        }
    }     
}

/**
 * ���㴫���ַ��Ľ�������
 * ���뿪ʼ���������м���ַ���
 * */
RCNode endPos(RCNode startPos, string str)
{
    RCNode nextPos = startPos;
    char *p = (char*)malloc(sizeof(str));
    strcpy(p, str);
    char *pp = p;
    char *enter;
    while(*pp)
    {
        if(*pp=='\n')
        {
            enter = pp;   //��λ���з�λ��
            nextPos.row ++;
        }
        pp ++;
    }
    nextPos.column = strlen(enter) - 1;   //���һ�е�����Ϊ���з����ַ���������
    free(p);
    return nextPos;
}