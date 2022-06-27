/**
 * 
 * ���ļ�����ʵ�ּ�¼��redo��undo�Ĳ���ʵ��
 * 
 */ 

#include "record.h"
#include "display.h"
#include "global.h"
#include "file.h"

static linkedList *nodeHead = NULL, *nodeTail = NULL;
static linkedList *curNode = NULL;
static int recordID = 0;    //��¼�Ĳ���ID

/**
 * ��ʼ��������¼
 * ��֮ǰ�м�¼���������ݣ���Ҫȫ���������
 */ 
void initRecord()
{   
    curNode = NULL;
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
    nodeHead = nodeTail = curNode = NULL;
}

/**
 * ����µ�record ID
 */ 
int newRecordID() {
    recordID = (recordID + 1) % (1 << 30);
    if (recordID == 0) recordID++;
    return recordID;
}

/**
 * ��¼����
 * op ��������: OP_ADD ����ַ��� OP_DELETE ɾ���ַ���
 * pos ����λ��
 * str ���/ɾ�����ַ���    ���ַ��������и��ģ���copy���ٴ���
 */ 
void record(int op, RCNode pos, const string str, int recordID) {
    if (recordID == 0) return;
    if (nodeHead == NULL)   //�ж�ͷ�ڵ��Ƿ�Ϊ��
    {
        nodeHead = (linkedList*)mallocDIY(sizeof(linkedList));
        nodeHead->op = op;
        nodeHead->pos = pos;
        nodeHead->str = str;
        nodeHead->id = recordID;
        nodeTail = nodeHead;
        curNode = nodeHead;
        nodeHead->last = NULL;
        nodeHead->next = NULL;
        setSaveState(0);
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
            nodeHead = (linkedList*)mallocDIY(sizeof(linkedList));
            nodeHead->op = op;
            nodeHead->pos = pos;
            nodeHead->str = str;
            nodeHead->id = recordID;
            nodeTail = nodeHead;
            curNode = nodeHead;
            nodeHead->last = NULL;
            nodeHead->next = NULL;
            setSaveState(0);
        }else
        {
            linkedList *temNode;
            temNode = (linkedList*)mallocDIY(sizeof(linkedList));  
            temNode->op = op;
            temNode->pos = pos;
            temNode->id = recordID;
            temNode->str = (string)mallocDIY(sizeof(char) * (strlen(str) + 1));
            temNode->next = NULL;
            strcpy(temNode->str, str);
            nodeTail->next = temNode;    //����˫������
            temNode->last = nodeTail;
            nodeTail = nodeTail->next;
            curNode = nodeTail;    //��ָ��ǰ�ڵ��ָ���Ƶ�����ĩ��
            setSaveState(0);
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
            setSaveState(0);

            //��λ���λ����ɾ���ַ��Ŀ�ʼλ��
            setCursorRC(curNode->pos);
            setSelectStartRC(curNode->pos);
            setSelectEndRC(curNode->pos);
            setCursorInWindow();

        }else if(curNode->op == OP_DELETE)   //�������Ϊɾ���ַ�����ɾ�����ַ��ӻ�ȥ
        {
            addContentByString(curNode->pos, curNode->str, 0);
            setSaveState(0);
            
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
        if (curNode->last != NULL && curNode->last->id == curNode->id) {    //���ǰһ�������ʹ˴β�����ͬһ�������������undo
            curNode = curNode->last;   //��ָ��ǰ������ָ��ǰ��һλ
            undo();
        } else {
            curNode = curNode->last;   //��ָ��ǰ������ָ��ǰ��һλ
        }
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
        setSaveState(0);

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
        setSaveState(0);
        
        //��λ���λ����ɾ���ַ����Ŀ�ʼλ��
        setCursorRC(curNode->pos);
        setSelectStartRC(curNode->pos);
        setSelectEndRC(curNode->pos);
        setCursorInWindow();
    }
    if (curNode->next != NULL && curNode->next->id == curNode->id) {    //�����һ�������ʹ˴β�����ͬһ�������������redo
        redo();
    }
}