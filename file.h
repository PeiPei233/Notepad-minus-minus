/*
    ��Ҫʵ���ļ���������ع���
*/
#ifndef __FILE_H__
#define __FILE_H__

#include "global.h"
/*
	��ʼ���ļ����� , һ��ʾͼ�ν����Ҫ���� 
*/
void initFileConfig();
/*
    ��һ���ļ�
    ���Ӧ��Ҫ��Windows����
    �򿪺���´������Ͻ�����
    ����currentString�ַ���
*/
void openFile();

/*
    �½�һ���ļ�
    �����½����ļ����ڱ���ʱҪѡ����λ��
*/
void createFile();


/*
    ���浱ǰ�򿪵��ļ�
    ������½����ļ� Ҫ��Windows����ѡ���λ��
    ����ʹ���֮ǰ��λ��
*/
void saveFile();

/*
    ��ȡ��ǰ�ı���״̬
    �Ƿ񱣴�
*/
int getSaveState();

/*
	���Ϊ 
	�����ʱ�ļ�������ʱд�ģ���ͬʱ���浱ʱ�ļ� 
*/
void saveAnother();
 
/*
    ���ñ���״̬
*/
void setSaveState(int newSaveState);

/*
	���ص�ǰ�ļ���
*/ 
char *getCurrentFileName();

/*
    �˳�ʱ��δ�������ṩѡ�� ѡ���Ƿ񱣴���� 
*/
void exitApplication();

#endif
