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

/*
    ���ݴ������ʼ������������ֹ����������и���
*/
void copyText()
{   
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();    
    char *cur = getCurrentString();  
    // printf("COPY1:%s\n", cur);
    int start, end;
    start = numofFormerWords(startSelect);   //���㸴���ַ�����ʼλ�ã�����λ��
    end = numofFormerWords(endSelect);
    if (start > end) {
        int t = start;
        start = end;
        end = t;
    }
    if (start == end) return;   //ѡ����ַ����򲻸���
    char *copystr = (char *) malloc(sizeof(char) * (end - start + 1));   //������Ҫ���Ƶ��ı�
    int length = end - start;
    //�õ�ѡ�и����ı�
    for (int i = 0; i < length; i++) {
        copystr[i] = cur[start + i];
    }
    copystr[length] = '\0';		//���ø����ı����������ı�������1��ʼ��
    // printf("COPY:%d %d %s\n", start, end, copystr);
    
    // printf("COPY2:%s\n", cur);
    //��ϵͳ���а���н���
    if (!OpenClipboard(NULL)|| !EmptyClipboard())
    {   
        printf("�򿪻���ռ��а����\n");
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
    // int start, end, length;
    // start = numofFormerWords(startSelect);
    // end = numofFormerWords(endSelect);
    // if (start > end) {
    //     int t = start;
    //     start = end;
    //     end = t;
    // }

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
        printf("�򿪼��а����\n");
        return ;   
    }
    HGLOBAL hMem = GetClipboardData(CF_TEXT);
    if(hMem)
    {
        //��ȡ�ַ���
        LPSTR lpStr = (LPSTR)GlobalLock(hMem);
        if(lpStr)
        {   
            pasteText = (char *) malloc(sizeof(char) * (strlen(lpStr) + 1));
            strcpy(pasteText, lpStr);   //�����а�����ݿ������ַ�������
            GlobalUnlock(hMem);   //�ͷ��ڴ���
        }
    }
    //�رռ��а�
    CloseClipboard();

    //��ԭ�ı�����ճ���޸�
    // char *curText = getCurrentString();
    /*char *cur = curText;
    char behindText[MAX];   //���屻ѡ���ı�����ַ���
    cur += (end+1);    //ָ��ָ��ѡ���ı���֮��ĵ�һ���ַ�
    strcpy(behindText, cur);
    curText[start-1] = '\0';     //����ѡ���ı�֮ǰ���ַ���������
    strcat(curText, pasteText);    //��ѡ���ı�֮ǰ�ַ�������ճ���ı�
    strcat(curText, behindText);    //��ճ������ı���ѡ���ı�֮����ַ�������
    */
    // string s;
    // s = Concat(SubString(curText, 0, start - 1), pasteText);
    // //�޸�currentString 
	// setCurrentString(Concat(s,SubString(curText, end+1, StringLength(curText))));
	// free(s);
    deleteSelectString();
    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);
    addString(pasteText);

    //���ĩ�˵�����λ��
    int i = numofFormerWords(startSelect), j = i + strlen(pasteText);
    string s = getCurrentString();
    int lens = strlen(s);
    while (i < lens && i < j) {
        if (s[i] == '\n') {
            startSelect.row++;
            startSelect.column = 1;
        } else startSelect.column++;
        i++;
    }
    
    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);

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

   /* //ɾ��ѡ������
    int start, end, length;
    start = numofFormerWords(startSelect);
    end = numofFormerWords(endSelect) - 1;
    length = end - start;
    char *curText = getCurrentString();
   char behindText[MAX];   //���屻ѡ���ı�����ַ�
    char *cur = curText;
    cur += (end+1);    //ָ��ָ��ѡ���ı���ĵ�һ���ַ�
    strcpy(behindText, cur);
    curText[start-1] = '\0';    //����ѡ���ı�֮ǰ���ַ���������
    strcat(curText, behindText);    //����ǰ���ַ��������м�ɾ��
    */
    //���޸ĺ���ı�����
	deleteSelectString();
    
    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }

    setSelectStartRC(startSelect);
    setSelectEndRC(startSelect);
    setCursorRC(startSelect);
    
}
