/*
    ��Ҫ�������� ���� ճ������
    ��һ����Ӧ��Ҫ��Windows��������н���
*/
#include <stdio.h>
#include <Windows.h>
#include "global.h"
#include "edit.h"
#include "file.h"


/*
    ���ݴ������ʼ������������ֹ����������и���
*/
void copyText()
{   
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();    
    char copystr[MAX];   //������Ҫ���Ƶ��ı�
    char *cur = getCurrentString();  
    int start, end;
    start = numofFormerWords(startSelect);   //���㸴���ַ�����ʼλ�ã�����λ��
    end = numofFormerWords(endSelect) - 1;
    cur += start;   // ָ��ָ��ѡ�и����ı���ʼ��
    strcpy(copystr, cur);   //�õ�ѡ�и����ı�
    int length = end - start + 1;
    copystr[length] = '\0';		//���ø����ı����������ı�������1��ʼ��

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
}

/*
    ���ݴ������ʼλ��ճ���ı����ݣ�����Ҫ�޸Ķ�Ӧ���ļ���
*/
void pasteText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    int start, end, length;
    start = numofFormerWords(startSelect);
    end = numofFormerWords(endSelect) - 1;
    length = end - start;  //����Ҫ���ǵ��ַ�����ʼλ�ã���ֹλ�ã�����

    //��ϵͳ���а彻����ȡ�������ݲ�������pasteText����
    char pasteText[MAX];
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
            strcpy(pasteText, lpStr);   //�����а�����ݿ������ַ�������
            GlobalUnlock(hMem);   //�ͷ��ڴ���
        }
    }
    //�رռ��а�
    CloseClipboard();

    //��ԭ�ı�����ճ���޸�
    char *curText = getCurrentString();
    char *cur = curText;
    char behindText[MAX];   //���屻ѡ���ı�����ַ���
    cur += (end+1);    //ָ��ָ��ѡ���ı���֮��ĵ�һ���ַ�
    strcpy(behindText, cur);
    curText[start-1] = '\0';     //����ѡ���ı�֮ǰ���ַ���������
    strcat(curText, pasteText);    //��ѡ���ı�֮ǰ�ַ�������ճ���ı�
    strcat(curText, behindText);    //��ճ������ı���ѡ���ı�֮����ַ�������
    
    //���޸ĺ��ı����뻺���ļ�
    FILE *currentFile;
    if(!(currentFile = fopen("currentFile.txt","w")))
    {
        printf("�򿪴���");
        return ;
    }
    fprintf(currentFile, "%s", curText);
    fclose(currentFile);
}

/*
    ���ݴ������ʼ������������ֹ����������м���
    ����Ҫ�ȸ���������ݣ��ٽ���ɾ��
*/
void shearText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    copyText();   //�ȶ�ѡ�����ݽ��и���

    //ɾ��ѡ������
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
    
    //���޸ĺ���ı�����
    FILE *currentFile;  
    if(!(currentFile = fopen("currentFile.txt","w")))
    {
        printf("�򿪴���");
        return ;
    }
    fprintf(currentFile, "%s", curText);  
    fclose(currentFile);
}