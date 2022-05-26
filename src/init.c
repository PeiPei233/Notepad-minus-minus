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
 * 初始化应用程序
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
	uiGetMouse(x, y, button, event);
	//在这里自定义鼠标回调函数
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
	uiGetChar(ch);
	//在这里自定义字符回调函数
	inputChar(ch);
	
	display();
}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	//在这里自定义键盘回调函数
	inputKeyboard(key, event);
	
	display();

}

#define REFRESH 1
void TimerEventProcess(int timerID) {
	//光标闪烁频率为0.5s，因此每0.5s修改更改光标状态的函数
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
