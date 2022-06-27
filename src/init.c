#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
#include "callback.h"
#include "style.h"
#include "display.h"
#include "file.h" 
#include "storage.h"

double winWidth, winHeight;

/**
 * ��ʼ��Ӧ�ó���
 */ 
void initApplication() {
	winHeight = GetWindowHeight();
	winWidth = GetWindowWidth();
	
	initStorage();
	initFileConfig();
	InitGUI();
	initDisplay();

	setCursorRC((RCNode) {1, 1});
	setSelectStartRC((RCNode) {1, 1});
	setSelectEndRC((RCNode) {1, 1});
	setWindowCurrentRC((RCNode) {1, 1});

}

void MouseEventProcess(int x, int y, int button, int event) {
    if (getProcessFileState()) {    //��������ڴ����ļ����򲻸��¡�
        return;
    }
	uiGetMouse(x, y, button, event);
	//�������Զ������ص�����
	if (event == BUTTON_DOWN && getTextDisplayState()) {
		display();
		getMouse(x, y, button, event);
		display();
	} else {
		getMouse(x, y, button, event);
		display();
	}

}

void CharEventProcess(char ch) {
    if (getProcessFileState()) {    //��������ڴ����ļ����򲻸��¡�
        return;
    }
	uiGetChar(ch);
	//�������Զ����ַ��ص�����
	inputChar(ch);
	
	display();
}

void KeyboardEventProcess(int key, int event) {
    if (getProcessFileState()) {    //��������ڴ����ļ����򲻸��¡�
        return;
    }
	uiGetKeyboard(key, event);
	//�������Զ�����̻ص�����
	inputKeyboard(key, event);
	
	display();

}

#define REFRESH 1
void TimerEventProcess(int timerID) {
    if (getProcessFileState()) {    //��������ڴ����ļ����򲻸��¡�
        return;
    }
	//�����˸Ƶ��Ϊ0.5s�����ÿ0.5s�޸ĸ��Ĺ��״̬�ĺ���
	if (timerID == REFRESH) {
		display();
	}
}

void Main() {

	SetWindowTitle("Notepad--");
	InitGraphics();
	// InitConsole();

	initApplication();

	registerCharEvent(CharEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);

	startTimer(REFRESH, 500);

}
