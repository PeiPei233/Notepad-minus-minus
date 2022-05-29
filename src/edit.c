/*
    ��Ҫ�������� ���� ճ������
    ��һ����Ӧ��Ҫ��Windows��������н���
*/
#include <stdio.h>
#include <Windows.h>
#include "global.h"
#include "edit.h"
#include "file.h"
#include "strlib.h"
#include <string.h>
#include "storage.h"
#include "display.h"
#include "record.h"
/*
    ���ݴ������ʼ������������ֹ����������и���
*/
void copyText()
{   
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();    
    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }else if(startSelect.row == endSelect.row && startSelect.column == endSelect.column)  //���û��ѡ�����ݣ������и��Ʋ���
    {
        return ;
    }
	char *copystr=getContent(startSelect, endSelect); 
    
    //��ϵͳ���а���н���
    if (!OpenClipboard(NULL)|| !EmptyClipboard())
    {   
        MessageBoxA(NULL,"�򿪻���ռ��а����","Notepad--",MB_OK | MB_ICONWARNING | MB_TASKMODAL);
        return ;   
    }
    HGLOBAL hMen;
    hMen = GlobalAlloc(GMEM_MOVEABLE, ((strlen(copystr)+1)*sizeof(TCHAR)));    // ����ȫ���ڴ�    
    // �����ݿ�����ȫ���ڴ���
    LPSTR lpStr = (LPSTR)GlobalLock(hMen);    // ����ס�ڴ���
    memcpy(lpStr, copystr, ((strlen(copystr))*sizeof(TCHAR)));    // �ڴ渴��
    lpStr[strlen(copystr)] = (TCHAR)0;    // �����ַ�������
    GlobalUnlock(hMen);   // �ͷ���
    SetClipboardData(CF_TEXT, hMen);    // ���ڴ��е����ݷŵ����а���
    CloseClipboard();  //�رռ��а�
    free(copystr);
    // printf("COPY3:%s\n", cur);
}

/*
    ���ݴ������ʼλ��ճ���ı����ݣ�����Ҫ�޸Ķ�Ӧ���ļ���
*/
void pasteText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }
    
    // length = end - start;  //����Ҫ���ǵ��ַ�����ʼλ�ã���ֹλ�ã�����

    //��ϵͳ���а彻����ȡ�������ݲ�������pasteText����
    char *pasteText;
    if(!OpenClipboard(NULL))
    {
        MessageBoxA(NULL,"�򿪼��а����","Notepad--",MB_OK | MB_ICONWARNING | MB_TASKMODAL);
        return ;   
    }
    HGLOBAL hMem = GetClipboardData(CF_TEXT);
    if(hMem == NULL)
    {
        // printf("�����ı�");
        return ;
    }
    //��ȡ�ַ���
    const char* lpStr = GlobalLock(hMem);
    if(lpStr == NULL)
    {
        MessageBoxA(NULL,"�޷���ȡ���а��ı�","Notepad--",MB_OK | MB_ICONWARNING | MB_TASKMODAL);
        return ;
    }
    pasteText = (char *) mallocDIY(sizeof(char) * (strlen(lpStr) + 1));
    strcpy(pasteText, lpStr);   //�����а�����ݿ������ַ�������
    GlobalUnlock(hMem);   //�ͷ��ڴ���
    //�رռ��а�
    CloseClipboard();
    
    int recordID = newRecordID();
    deleteContent(startSelect,endSelect,recordID);
    addContent(BY_STRING,startSelect,pasteText,recordID);  //ճ��Ŀ���ַ��� 
    setSaveState(0);

    //���ĩ�˵�����λ��
    int lens = strlen(pasteText);
	int i=0;
    while (i < lens) {
        if (pasteText[i] == '\n') {
            startSelect.row++;
            startSelect.column = 1;
        } else startSelect.column++;
        i++;
    }

    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);
    
    setCursorInWindow(); 

    free(pasteText);
}

/*
    ���ݴ������ʼ������������ֹ����������м���
    ����Ҫ�ȸ���������ݣ��ٽ���ɾ��
*/
void shearText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    copyText();   //�ȶ�ѡ�����ݽ��и���

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }
    //���޸ĺ���ı�����
	deleteContent(startSelect,endSelect,newRecordID());
    setSaveState(0);
     //����ѡ��Χ 
    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);

    setCursorInWindow();    

}
