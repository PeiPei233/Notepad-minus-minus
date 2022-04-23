#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
#include "callback.h"
//�������������Ҫ���Զ����ͷ�ļ�

double winWidth, winHeight;

void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event);
	//�������Զ������ص�����
	getMouse(x, y, button, event);
}

void CharEventProcess(char ch) {
	uiGetChar(ch);
	//�������Զ����ַ��ص�����
	inputChar(ch);
}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	//�������Զ�����̻ص�����
	inputKeyboard(key, event);

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