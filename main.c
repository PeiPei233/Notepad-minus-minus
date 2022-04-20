#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "imgui.h"

double winHeight, winWidth;

void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event);
	display();
}

void CharEventProcess(char ch) {
	uiGetChar(ch);
	display();
}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	display();
}

void TimerEventProcess(int timerID) {
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

	display();

}

void display() {
    
}