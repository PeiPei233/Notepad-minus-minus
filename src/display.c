/*
    主要包括文本的绘制 光标绘制 查找替换窗口的绘制 格式字体选择的绘制
*/

#include "display.h"

/*
    画菜单 并根据菜单选项执行对应的函数和命令
    在没有打开任何文件时显示新建文件
*/
void drawMenu() {

}

/*
    在范围内画文本，建议的参数包括左上角坐标(x, y) 矩形的长宽(w, h) 行高 是否需要自动换行 等等
*/
void drawTextWithinArea(double x, double y, double w, double h) {

}

/*
    根据传入的在文本框窗口中的行列位置(r, c)显示光标
    需要的参数包括但不仅限于文本框左上角坐标，行高等等
*/
void drawCursor(double rowHeight) {
    RCNode cursor = getCursorRC();
}

/*
    根据选中的范围绘制高亮文本底色
    注意一下要先绘制文本底色 再绘制文本
    参数包括但不仅限于选中范围的起始行列坐标与终止行列坐标
*/
void drawSelectRange(double rowHeight) {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
}

/*
    绘制查找窗口 包括一个查找内容文本框和查找按钮等
    建议绘制在文件文本的右上角
    先绘制底层文本 在绘制查找窗口时先绘制一个白色矩形覆盖底下的文本，在绘制查找窗口
    绘制替换窗口同理
*/
void drawFindArea(double x, double y, double w, double h) {

}

/*
    绘制替换窗口 包括两个文本框（分别是查找和替换）以及替换按钮等
*/
void drawReplaceArea(double x, double y, double w, double h) {

}