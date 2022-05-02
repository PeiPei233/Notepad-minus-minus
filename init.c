/*
    ���ļ���Ҫ�����򿪳���ʱ�ĳ�ʼ��ͼ�ν��棬�����˵����ı���ʾ�ȵȡ�
    �ǵ��޸Ķ�Ӧ��ͬ��ͷ�ļ����ټӵ�main��include��
*/

#include <stdio.h>
#include "global.h"
#include "display.h"
#include "libgraphics.h"
#include "imgui.h"
//��Ҫ��Ҫʵ�����º���

/*
    ��ʼ�������Զ�����ɫ
*/
void initColor() {
    DefineColor("Button Gray", 243 * 1./255, 243 * 1./255, 243 * 1./255);
    DefineColor("Button Hot Gray", 225 * 1./255, 225 * 1./255, 225 * 1./255);
    DefineColor("Menu Gray", 221 * 1./255, 221 * 1./255, 221 * 1./255);
    DefineColor("Menu Hot Gray", 198 * 1./255, 198 * 1./255, 198 * 1./255);
    DefineColor("Textbox Hot Blue", 0, 144 * 1./255, 241 * 1./255);
    DefineColor("Select Blue", 173 * 1./255, 214 * 1./255, 255 * 1./255);
}

/*
    ��ʼ��չʾ����
*/
void initDisplay() {
    initColor();
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);
    setMenuColors("Menu Gray", "Black", "Menu Hot Gray", "Black", 1);
    SetFont("Fira Code");
    display();
}