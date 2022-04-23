#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
//�������������Ҫ���Զ����ͷ�ļ�

void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event);
	//�������Զ������ص�����

}

void CharEventProcess(char ch) {
	uiGetChar(ch);
	//�������Զ����ַ��ص�����

}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	//�������Զ�����̻ص�����

}

void TimerEventProcess(int timerID) {
	//�����˸Ƶ��Ϊ0.5s�����ÿ0.5s�޸ĸ��Ĺ��״̬�ĺ���
}

void Main() {
	InitGraphics();
	InitConsole();

	winHeight = GetWindowHeight();
	winWidth = GetWindowWidth();

	registerCharEvent(CharEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);

}