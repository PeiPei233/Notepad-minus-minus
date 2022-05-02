#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
#include "callback.h"
#include "init.h"
//在这里添加所需要的自定义的头文件

double winWidth, winHeight;

void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event);
	//在这里自定义鼠标回调函数
	getMouse(x, y, button, event);

	display();
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