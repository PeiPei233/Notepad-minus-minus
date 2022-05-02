#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
#include "callback.h"
#include "init.h"
//�������������Ҫ���Զ����ͷ�ļ�

double winWidth, winHeight;

void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event);
	//�������Զ������ص�����
	getMouse(x, y, button, event);

	display();
}

void CharEventProcess(char ch) {
	uiGetChar(ch);
	//�������Զ����ַ��ص�����
	inputChar(ch);
	
	display();
}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	//�������Զ�����̻ص�����
	inputKeyboard(key, event);
	
	display();

}

#define REFRESH 1
void TimerEventProcess(int timerID) {
	//�����˸Ƶ��Ϊ0.5s�����ÿ0.5s�޸ĸ��Ĺ��״̬�ĺ���
	if (timerID == REFRESH) {
		display();
	}
}

void Main() {

	SetWindowTitle("Notepad--");
	InitGraphics();
	InitConsole();
	
	winHeight = GetWindowHeight();
	winWidth = GetWindowWidth();

	initDisplay();

	registerCharEvent(CharEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);

	startTimer(REFRESH, 500);

}