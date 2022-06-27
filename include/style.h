/*
    ���ļ���Ҫ�����򿪳���ʱ�ĳ�ʼ��ͼ�ν��棬�����˵����ı���ʾ�ȵȡ�
    �ǵ��޸Ķ�Ӧ��ͬ��ͷ�ļ����ټӵ�main��include��
*/

#ifndef __STYLE_H__
#define __STYLE_H__

#include "strlib.h"

//��Ҫ��Ҫʵ�����º���
typedef struct {
    string fontFamily;
    int fontSize;
    double lineSpacing;
    string textColor;
    string backgroundColor;
}TextStyle;

/*
    ��ʼ�������Զ�����ɫ
*/
void initColor();

/**
 * ��ȡ��ǰ����������ʽ
 */ 
TextStyle getTextStyle();

/**
 * ���õ�ǰ����������ʽ
 */ 
void setTextStyle(TextStyle newTextStyle);

/**
 * ��ʮ��������ɫ�붨������ɫ
 */
int defineColorRGB(char *des, char *rgb);

//�����������ʼ��

/*
    ��ʼ��չʾ����
*/
void initDisplay();

/*
    ˢ����ʾ
*/
void display();

#endif