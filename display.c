/*
    主要包括文本的绘制 光标绘制 查找替换窗口的绘制 格式字体选择的绘制
*/

#include "display.h"
#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
#include "file.h"
#include "edit.h"
#include "find.h"
#include "time.h"
#include "style.h"
#include "callback.h"
#include "storage.h"
#include "record.h"
#include <math.h>
#include <windows.h>
#include <string.h>

static int isShowFind = 0;          //是否显示查找框
static int isShowReplace = 0;       //是否显示替换框
static int isShowSetting = 0;       //是否显示设置页面
static int isShowKeyboard = 0;      //是否显示键盘快捷键页面
static int isShowAbout = 0;         //是否显示关于页面
static int isShowContextMenu = 0;   //是否显示右键菜单

//右键菜单坐标
static double contextMenuX;
static double contextMenuY;

static int lockDisplay = 0;     //用来给display上锁，防止上一个还没结束又开始display

static char inputFindText[100010] = "";
static char inputReplaceText[100010] = "";

/**
 * 获取查找窗口显示状态
 */ 
int getFindDisplayState() {
    return isShowFind;
}

/**
 * 获取替换窗口显示状态
 */ 
int getReplaceDisplayState() {
    return isShowReplace;
}

/**
 * 获取右键菜单显示状态
 */ 
int getContextMenuDisplayState() {
    return isShowContextMenu; 
}

/**
 * 更改查找窗口显示状态
 */ 
void setFindDisplayState(int newFindDisplayState) {
    isShowFind = newFindDisplayState;
}

/**
 * 更改替换窗口显示状态
 */ 
void setReplaceDisplayState(int newReplaceDisplayState) {
    isShowReplace = newReplaceDisplayState;
}

/**
 * 更改右键菜单显示状态
 */ 
void setContextMenuDisplayState(int newContextMenuDisplayState) {
    isShowContextMenu = newContextMenuDisplayState;
}

/**
 * 获取文本区域显示状态
 */
int getTextDisplayState() {
    return !(isShowAbout || isShowSetting || isShowKeyboard);
}

static int totDisplayRow;

/**
 * 获取当前显示的总行数
 */ 
int getTotalDisplayRow() {
    return totDisplayRow;
}

/**
 * 更新当前显示的总行数
 */ 
void updateTotalDisplayRow() {    
    string originFont = GetFont();
    string originColor=  GetPenColor();
    int originPointSize = GetPointSize();
    TextStyle style = getTextStyle();

    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double menuBarH = fH * 1.5;
    double minY = fH * 1.4;

    SetFont(style.fontFamily);
    SetPointSize(style.fontSize);

    fH = GetFontHeight();
    double x = menuBarH / 1.5 * 2 / 3;
    double th = fH * style.lineSpacing;
    double y = winHeight - menuBarH - th * 1.25;
    totDisplayRow = ceil((y + GetFontAscent() - minY) / th);
    
    SetPointSize(originPointSize);
    SetPenColor(originColor);
    SetFont(originFont);
}

/*
    画菜单 并根据菜单选项执行对应的函数和命令
    在没有打开任何文件时显示新建文件
*/
static void drawMenu() {

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    char *menuListFile[] = {"文件",
        "新建                       Ctrl + N",
        "打开                       Ctrl + O",
        "保存                       Ctrl + S",
        "另存为         Ctrl + Shift + S",
        "退出                       Ctrl + W"
    };

    double fH = GetFontHeight();
    double x = 0;
    double y = winHeight;
    double h = fH * 1.5;
    double w = TextStringWidth(menuListFile[0]) * 2;
    double wlist = TextStringWidth(menuListFile[1]) * 1.13;

    char *originColor = GetPenColor();
    SetPenColor("Menu Gray");
    drawRectangle(x, y - h, winWidth, h, 1);
    SetPenColor(originColor);

    int selection;
    selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
    switch (selection) {
        case 1:     //新建
            createFile();
            break;
        case 2:     //打开
            openFile();
            break;
        case 3:     //保存
            saveFile();
            break;
        case 4:     //保存
            saveAnother();
            break;
        case 5:     //退出
            exitApplication();
            break;
    }

    char *menuListEdit[] = {"编辑",
        "撤销                       Ctrl + Z",
        "重做                       Ctrl + Y",
        "剪切                       Ctrl + X",
        "复制                       Ctrl + C",
        "粘贴                       Ctrl + V",
        "查找                       Ctrl + F",
        "替换                       Ctrl + H",
        "全选                       Ctrl + A"
    };

    selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
    switch (selection) {
        case 3:     //剪切
            shearText();
            break;
        case 4:     //复制
            copyText();
            break;
        case 5:     //粘贴
            pasteText();
            break;
        case 6:     //查找
            isShowFind ^= 1;
            break;
        case 7:     //替换
            isShowReplace ^= 1;
            break;
        case 8:     //全选
        {
            int totr = getTotalRow();
            int totc = getRowLength(totr) + 1;
            setSelectStartRC((RCNode) {1, 1});
            setSelectEndRC((RCNode) {totr, totc});
            setCursorRC((RCNode) {totr, totc});
            break;
        }
        case 1:     //撤销
            undo();
            break;
        case 2:     //重做
            redo();
            break;
    }

    char *menuListSetting[] = {"首选项",
        "设置                       Ctrl + E",
        "键盘快捷方式          Ctrl + K",
        "关于 Notepad--",
        "帮助                               F1"
    };

    selection = menuList(GenUIID(0), x + w * 2, y - h, w, wlist, h, menuListSetting, sizeof(menuListSetting) / sizeof(menuListSetting[0]));
    switch (selection) {
        case 1:     //设置
            isShowSetting ^= 1;
            break;
        case 2:     //键盘快捷方式
            isShowKeyboard ^= 1;
            break;
        case 3:     //关于
            isShowAbout ^= 1;
            break;
        case 4:     //帮助
            WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
            break;
    }

    //文件名
    SetPenColor("Black");
    string fileName;
    static char name[10010];
    if (!(fileName = getCurrentFileName())) {
        if (getSaveState()) sprintf(name, "无标题 - Notepad--");
        else sprintf(name, "无标题* - Notepad--");
    } else {
        if (getSaveState()) sprintf(name, "%s - Notepad--", fileName);
        else sprintf(name, "%s* - Notepad--", fileName);
    }
    double tw = TextStringWidth(name);
    double tx = max(winWidth / 2 - tw / 2, x + w * 3.5);
    int i = strlen(name) - 1;
    while (tx + TextStringWidth(name) > winWidth - h * 2 / 3 && i >= 0) {    //名字过长则最后显示省略号...
        if (name[i] & 0x80) i--;
        i--;
        name[i + 1] = name[i + 2] = name[i + 3] = '.';
        name[i + 4] = 0;
    }
    MovePen(tx, winHeight - h / 2 - GetFontAscent() / 2);
    DrawTextString(name);
    // printf("%s\n", name);

    //状态栏部分
    SetPenColor("Menu Gray");
    drawRectangle(0, 0, winWidth, fH * 1.4, 1);
    static char s[110];
    RCNode cursor = getCursorRC();
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    if (startSelect.column == endSelect.column && startSelect.row == endSelect.row) {
        sprintf(s, "行 %d，列 %d", cursor.row, cursor.column);
        SetPenColor("Black");
        MovePen(TextStringWidth("    "), GetFontDescent() + fH * 0.2);
        DrawTextString(s);
    } else {
        int len = getContentLength(getSelectStartRC(), getSelectEndRC());
        sprintf(s, "行 %d，列 %d（已选择%d）", cursor.row, cursor.column, len);
        SetPenColor("Black");
        MovePen(TextStringWidth("    "), GetFontDescent() + fH * 0.2);
        DrawTextString(s);
    }
    

    MovePen(winWidth - TextStringWidth("            ") - TextStringWidth("GB 2312"), GetFontDescent() + fH * 0.2);
    DrawTextString("GB 2312");

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}

static unsigned int capT = 0;
static char *t;     //某一行的字符
static double *textWidth;   //textWidth[i]: [0, i)区间的字符串长度

/*
    绘制文本区域
*/
static void drawTextArea() {
    RCNode winCurrent = getWindowCurrentRC();
    RCNode cursor = getCursorRC();
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    
    string originFont = GetFont();
    string originColor=  GetPenColor();
    int originPointSize = GetPointSize();
    TextStyle style = getTextStyle();
    
    // printf("DISPLAYSTYLE:%s %d %lf\n", style.fontFamily, style.fontSize, style.lineSpacing);

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }

    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double menuBarH = fH * 1.5;
    double minY = fH * 1.4;

    SetFont(style.fontFamily);
    SetPointSize(style.fontSize);
    if (!defineColorRGB("Text Color", style.textColor)) {
        DefineColor("Text Color", 0, 0, 0);
    }
    if (!defineColorRGB("Background Color", style.backgroundColor)) {
        DefineColor("Background Color", 1, 1, 1);
    }
    SetPenColor("Background Color");
    drawRectangle(0, 0, winWidth, winHeight, 1);

    fH = GetFontHeight();
    double x = menuBarH / 1.5 * 2 / 3;
    double th = fH * style.lineSpacing;
    double y = winHeight - menuBarH - th * 1.25;
    int totl = ceil((y + GetFontAscent() - minY) / th);
    
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    if (getTotalRow() == 1 && getRowLength(1) == 0) {    //空字符串
        if (clock() % 1000 < 500 && getTypingState()) {
            int originPenSize = GetPenSize();
            SetPenColor("Text Color");
            SetPenSize(2);
            MovePen(x - dx, y - GetFontDescent());
            DrawLine(0, th);
            SetPenSize(originPenSize);
        }
    }

    for (int row = winCurrent.row - 1; row < winCurrent.row + totl; row++) {    //row:当前显示的行
        string s = getRowContent(row);
        if (s == NULL) continue;
        int i = 0, j = 0, lastj = 0, lens = getRowLength(row);  //i:原数组s的下标，j:处理制表符后新数组的下标
        int starti = -1, endi = -1;   //可显示部分的开始/结束的s的下标i
        if (lens && s[lens - 1] == '\n') lens--;
        unsigned int cntT = 0;   //该行内'\t'的数量
        while (i < lens) {
            if (s[i] == '\t') {
                cntT++;
            }
            i++;
        }
        i = 0;
        if (!capT) {    //没初始化过，重新初始化
            capT = 1024;
            while (capT < lens + cntT * 4 + 5) {
                capT <<= 1;
            }
            t = (char *) malloc(sizeof(char) * capT);
            textWidth = (double *) malloc(sizeof(double) * capT);
        } 
        else if (capT < lens + cntT * 4 + 5) {  //装不下了，重新初始化
            while (capT < lens + cntT * 4 + 5) {
                capT <<= 1;
            }
            free(t);
            free(textWidth);
            t = (char *) malloc(sizeof(char) * capT);
            textWidth = (double *) malloc(sizeof(double) * capT);
        }
        t[0] = t[1] = 0;
        textWidth[0] = 0;
        while (i < lens) {      //i:在s中的下标 j:在t中的下标位置
            if (s[i] == '\t') {     //处理制表符
                i++;
                if (j % 4 == 0) {
                    for (int _i = 1; _i <= 4; _i++) {
                        t[j] = ' ';
                        j++;
                    }
                    t[j] = t[j + 1] = 0;
                } else {
                    while (j % 4) {
                        t[j] = ' ';
                        j++;
                    }
                    t[j] = t[j + 1] = 0;
                }
                textWidth[i] = textWidth[i - 1] + TextStringWidth(t + lastj);
                textWidth[i + 1] = 0;
                lastj = j;
                if (x - dx + textWidth[i] > 0 && starti == -1) {
                    starti = i - 1;
                }
                if (x - dx + textWidth[i] > winWidth && endi == -1) {
                    endi = i;
                }
            } else if (s[i] & 0x80) {
                t[j] = s[i];
                i++;
                j++;
                t[j] = t[j + 1] = 0;
                
                t[j] = s[i];
                i++;
                j++;
                t[j] = t[j + 1] = 0;
                
                textWidth[i - 1] = textWidth[i - 2];
                textWidth[i] = textWidth[i - 1] + TextStringWidth(t + lastj);
                textWidth[i + 1] = 0;
                lastj = j;
                if (x - dx + textWidth[i] > 0 && starti == -1) {
                    starti = i - 2;
                }
                if (x - dx + textWidth[i] > winWidth && endi == -1) {
                    endi = i;
                }

            } else {
                t[j] = s[i];
                i++;
                j++;
                t[j] = t[j + 1] = 0;
                textWidth[i] = textWidth[i - 1] + TextStringWidth(t + lastj);
                textWidth[i + 1] = 0;
                lastj = j;
                if (x - dx + textWidth[i] > 0 && starti == -1) {
                    starti = i - 1;
                }
                if (x - dx + textWidth[i] > winWidth && endi == -1) {
                    endi = i;
                    while (i <= lens) {
                        textWidth[i] = textWidth[endi];
                        i++;
                    }
                    i--;
                    break;
                }
            }
        }
        if (lens == 0) starti = endi = 0;
        //draw select area
        if (s[i] == '\n') {     //如果选择部分有'\n'，则以空格表示出来
            if (row != endSelect.row) {
                lens++;
                textWidth[lens] = textWidth[lens - 1] + TextStringWidth(" ");
            }
        }

        SetPenColor("Select Blue");
        if (row > startSelect.row && row < endSelect.row) {     //该行在选择的行中间
            double w = textWidth[lens];
            double sx = max(0, x - dx);
            double ex = min(winWidth, x - dx + w);
            if (ex - sx != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        } else if (row == startSelect.row && startSelect.row != endSelect.row) {    //该行在选择的第一行
            // printf("SELECT:(%d, %d) TO(%d, %d)\n", startSelect.row, startSelect.column, endSelect.row, endSelect.column);
            double w = textWidth[lens] - textWidth[startSelect.column - 1];
            double sx = max(0, x - dx + textWidth[startSelect.column - 1]);
            double ex = min(winWidth, x - dx + textWidth[startSelect.column - 1] + w);
            if (w != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        } else if (row == endSelect.row && startSelect.row != endSelect.row) {      //该行在选择的最后一行
            double w = textWidth[endSelect.column - 1];
            double sx = max(0, x - dx);
            double ex = min(winWidth, x - dx + w);
            if (w != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        } else if (row == startSelect.row && startSelect.row == endSelect.row) {    //选择部分在同一行
            double w = textWidth[endSelect.column - 1] - textWidth[startSelect.column - 1];
            double sx = max(0, x - dx + textWidth[startSelect.column - 1]);
            double ex = min(winWidth, x - dx + textWidth[startSelect.column - 1] + w);
            if (w != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        }

        //draw text
        SetPenColor("Text Color");
        MovePen(x - dx + textWidth[starti], y - th * (row - winCurrent.row));
        if (endi != -1) t[endi] = 0;
        printf("I:%d %d\n", starti, endi);
        DrawTextString(t + starti);

        //draw cursor
        if (clock() % 1000 < 500 && getTypingState()) {     //500ms的间隔闪烁，不在输入状态时不显示光标
            int originPenSize = GetPenSize();
            SetPenSize(2);
            if (row == cursor.row) {
                double sx = x - dx + textWidth[cursor.column - 1];
                if (sx >= 0 && sx <= winWidth) {
                    MovePen(sx, y - th * (row - winCurrent.row) - GetFontDescent());
                    DrawLine(0, th);
                }
            }
            SetPenSize(originPenSize);
        }
    }

    SetPointSize(originPointSize);
    SetPenColor(originColor);
    SetFont(originFont);
}

static int findResult = 1;

/*
    绘制查找窗口 包括一个查找内容文本框和查找按钮等
    建议绘制在文件文本的右上角
    先绘制底层文本 在绘制查找窗口时先绘制一个白色矩形覆盖底下的文本，在绘制查找窗口
    绘制替换窗口同理
*/
static void drawFindArea() {
    
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double x = winWidth / 2;
    double y = winHeight - fH * 3.5;
    double w = winWidth * 15 / 32;
    double h = fH * 1.6;

    char *originColor = GetPenColor();
    SetPenColor("Button Gray");
    drawRectangle(x, y, w, h, 1);
    SetPenColor("Light Gray");
    drawRectangle(x, y, w, h, 0);
    SetPenColor(originColor);

    //展开成替换窗口的按钮
    double buttonFindToReplaceX = x + fH * 0.2;
    double buttonFindToReplaceY = y + fH * 0.2;
    double buttonFindToReplaceW = fH * 1.2;
    double buttonFindToReplaceH = fH * 1.2;

    //查找文本框
    double textboxFindX = x + fH * 1.6;
    double textboxFindY = buttonFindToReplaceY;
    double textboxFindW = w / 2;
    double textboxFindH = buttonFindToReplaceH;

    //关闭查找框按钮
    double buttonCloseX = x + w - fH * 1.4;
    double buttonCloseY = textboxFindY;
    double buttonCloseW = fH * 1.2;
    double buttonCloseH = textboxFindH;

    //查找上一个按钮
    double buttonFindLastX = buttonCloseX - fH * 1.7;
    double buttonFindLastY = textboxFindY;
    double buttonFindLastW = textboxFindH;
    double buttonFindLastH = textboxFindH;

    //查找下一个按钮
    double buttonFindNextX = buttonFindLastX - fH * 1.7;
    double buttonFindNextY = textboxFindY;
    double buttonFindNextW = textboxFindH;
    double buttonFindNextH = textboxFindH;

    //输入查找内容的文本框
    SetPenColor("White");
    drawRectangle(textboxFindX, textboxFindY, textboxFindW, textboxFindH, 1);
    SetPenColor(originColor);
    SetPointSize(13);
    if (textbox(GenUIID(0), textboxFindX, textboxFindY, textboxFindW, textboxFindH, inputFindText, sizeof(inputFindText) / sizeof(inputFindText[0]))) {
        
    }
    
    //查找下一个按钮
    SetPointSize(16);
    if (button(GenUIID(0), buttonFindNextX, buttonFindNextY, buttonFindNextW, buttonFindNextH, "↓")) {
        findResult = findNextText(inputFindText);
    }

    //查找上一个按钮
    if (button(GenUIID(0), buttonFindLastX, buttonFindLastY, buttonFindLastW, buttonFindLastH, "↑")) {
        findResult = findLastText(inputFindText);
    }

    //如果查找无结果则显示无结果
    if (!findResult) {
        SetPointSize(13);
        MovePen(textboxFindX + textboxFindW + fH * 0.5, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        SetPenColor("Red");
        DrawTextString("无结果");
        SetPenColor(originColor);
    }
    
    //如果查找的文本框里没有内容，则显示灰色文字 查找 提示
    if (strlen(inputFindText) < 1) {
        SetPenColor("Dark Gray");
        SetPointSize(13);
        MovePen(textboxFindX + GetFontAscent() / 2, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        DrawTextString("查找");
        SetPenColor(originColor);
    }
    
    //展开成替换窗口
    SetPointSize(18);
    if (button(GenUIID(0), buttonFindToReplaceX, buttonFindToReplaceY, buttonFindToReplaceW, buttonFindToReplaceH, "∨")) {
        isShowFind = 0;
        isShowReplace = 1;
        SetFont(originFont);
        display();
        return;
    }

    //关闭按钮
    if (button(GenUIID(0), buttonCloseX, buttonCloseY, buttonCloseW, buttonCloseH, "×")) {
        isShowFind = 0;
        setTypingState(1);
        SetFont(originFont);
        SetPointSize(originPointSize);
        display();
        return;
    }

    SetFont(originFont);
    SetPointSize(originPointSize);

}

/*
    绘制替换窗口 包括两个文本框（分别是查找和替换）以及替换按钮等
*/
static void drawReplaceArea() {

    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double x = winWidth / 2;
    double y = winHeight - fH * 4.9;
    double w = winWidth * 15 / 32;
    double h = fH * 3;

    char *originColor = GetPenColor();
    SetPenColor("Button Gray");
    drawRectangle(x, y, w, h, 1);
    SetPenColor("Light Gray");
    drawRectangle(x, y, w, h, 0);
    SetPenColor(originColor);

    //替换输入文本框
    double textboxReplaceX = x + fH * 1.6;
    double textboxReplaceY = y + fH * 0.2;
    double textboxReplaceW = w / 2;
    double textboxReplaceH = fH * 1.2;

    //显示查找窗口按钮
    double buttonReplaceToFindX = x + fH * 0.2;
    double buttonReplaceToFindY = y + fH * 0.2;
    double buttonReplaceToFindW = fH * 1.2;
    double buttonReplaceToFindH = fH * 2.6;

    //查找输入文本框
    double textboxFindX = x + fH * 1.6;
    double textboxFindY = y + fH * 0.2 + fH * 1.4;
    double textboxFindW = w / 2;
    double textboxFindH = fH * 1.2;

    //关闭按钮
    double buttonCloseX = x + w - fH * 1.4;
    double buttonCloseY = textboxFindY;
    double buttonCloseW = fH * 1.2;
    double buttonCloseH = textboxFindH;

    //查找上一个按钮
    double buttonFindLastX = buttonCloseX - fH * 1.7;
    double buttonFindLastY = textboxFindY;
    double buttonFindLastW = textboxFindH;
    double buttonFindLastH = textboxFindH;

    //查找下一个按钮
    double buttonFindNextX = buttonFindLastX - fH * 1.7;
    double buttonFindNextY = textboxFindY;
    double buttonFindNextW = textboxFindH;
    double buttonFindNextH = textboxFindH;

    //替换按钮
    const char *replaceButton = "替换";
    double buttonReplaceX = textboxReplaceX + textboxReplaceW + fH * 0.5;
    double buttonReplaceY = textboxReplaceY;
    double buttonReplaceW = TextStringWidth(replaceButton) * 1.3;
    double buttonReplaceH = textboxReplaceH;

    //替换全部按钮
    const char * replaceAllButton = "替换全部";
    double buttonReplaceAllX = buttonReplaceX + buttonReplaceW + fH;
    double buttonReplaceAllY = textboxReplaceY;
    double buttonReplaceAllW = TextStringWidth(replaceAllButton) * 1.2;
    double buttonReplaceAllH = textboxReplaceH;

    //只显示查找窗口
    SetPointSize(18);
    if (button(GenUIID(0), buttonReplaceToFindX, buttonReplaceToFindY, buttonReplaceToFindW, buttonReplaceToFindH, "∧")) {
        isShowFind = 1;
        isShowReplace = 0;
        SetFont(originFont);
        display();
        return;
    }

    //关闭替换窗口
    if (button(GenUIID(0), buttonCloseX, buttonCloseY, buttonCloseW, buttonCloseH, "×")) {
        isShowReplace = 0;
        isShowFind = 0;
        setTypingState(1);
        SetFont(originFont);
        SetPointSize(originPointSize);
        display();
        return;
    }

    //输入查找内容的文本框
    SetPenColor("White");
    drawRectangle(textboxFindX, textboxFindY, textboxFindW, textboxFindH, 1);
    SetPenColor(originColor);
    SetPointSize(13);
    if (textbox(GenUIID(0), textboxFindX, textboxFindY, textboxFindW, textboxFindH, inputFindText, sizeof(inputFindText) / sizeof(inputFindText[0]))) {
        
    }
    
    //查找下一个按钮
    SetPointSize(16);
    if (button(GenUIID(0), buttonFindNextX, buttonFindNextY, buttonFindNextW, buttonFindNextH, "↓")) {
        findResult = findNextText(inputFindText);
    }

    //查找上一个按钮
    if (button(GenUIID(0), buttonFindLastX, buttonFindLastY, buttonFindLastW, buttonFindLastH, "↑")) {
        findResult = findLastText(inputFindText);
    }
    
    //如果查找的文本框里没有内容，则显示灰色文字 查找 提示
    if (strlen(inputFindText) < 1) {
        SetPenColor("Dark Gray");
        SetPointSize(13);
        MovePen(textboxFindX + GetFontAscent() / 2, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        DrawTextString("查找");
        SetPenColor(originColor);
    }

    //显示替换文本输入文本框
    SetPointSize(13);
    SetPenColor("White");
    drawRectangle(textboxReplaceX, textboxReplaceY, textboxReplaceW, textboxReplaceH, 1);
    SetPenColor(originColor);
    if (textbox(GenUIID(0), textboxReplaceX, textboxReplaceY, textboxReplaceW, textboxReplaceH, inputReplaceText, sizeof(inputReplaceText) / sizeof(inputReplaceText[0]))) {
        
    }
    
    if (strlen(inputReplaceText) < 1) {
        SetPenColor("Dark Gray");
        MovePen(textboxReplaceX + GetFontAscent() / 2, textboxReplaceY + textboxReplaceH / 2 - GetFontAscent() / 2);
        DrawTextString("替换");
        SetPenColor(originColor);
    }

    //如果查找无结果则显示无结果
    if (!findResult) {
        SetPointSize(13);
        MovePen(textboxFindX + textboxFindW + fH * 0.5, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        SetPenColor("Red");
        DrawTextString("无结果");
        SetPenColor(originColor);
    }

    //显示替换按钮
    if (button(GenUIID(0), buttonReplaceX, buttonReplaceY, buttonReplaceW, buttonReplaceH, replaceButton)) {
        replaceText(inputFindText, inputReplaceText);
    }

    //显示替换全部按钮
    if (button(GenUIID(0), buttonReplaceAllX, buttonReplaceAllY, buttonReplaceAllW, buttonReplaceAllH, replaceAllButton)) {
        replaceAll(inputFindText, inputReplaceText);
    }

    SetFont(originFont);
    SetPointSize(originPointSize);

}

static char fontFamily[10010];
static char fontSize[10010];
static char lineSpacing[10010];
static char backgroundColor[10010];
static char textColor[10010];
static int initSetting = 1;     //1: 需要初始化
static TextStyle tmpStyle;

static void drawSettingPage() {
    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);
    setTextBoxColors("Menu Hot Gray", "Black", "Textbox Hot Blue", "Black", 0);

    double fH = GetFontHeight();
    double x = 0;
    double y = winHeight;
    double h = fH * 1.5;

    char *originColor = GetPenColor();
    SetPenColor("Menu Gray");
    drawRectangle(x, y - h, winWidth, h, 1);
    SetPenColor(originColor);

    setButtonColors("Menu Gray", "Black", "Menu Hot Gray", "Black", 1);

    tmpStyle = getTextStyle();

    if (initSetting) {
        initSetting = 0;
        strcpy(fontFamily, tmpStyle.fontFamily);
        strcpy(fontSize, IntegerToString(tmpStyle.fontSize));
        strcpy(lineSpacing, RealToString(tmpStyle.lineSpacing));
        strcpy(backgroundColor, tmpStyle.backgroundColor);
        strcpy(textColor, tmpStyle.textColor);
    }

    SetPointSize(40);
    MovePen(winWidth / 16, winHeight * 6 / 7);
    DrawTextString("设置");

    x = winWidth / 8;
    y = winHeight * 6 / 8;
    SetPointSize(16);
    double w = TextStringWidth("字体系列：") * 1.1;
    double tw = winWidth / 3;
    fH = GetFontHeight();
    double fD = GetFontDescent();
    double th = fH * 1.2;
    h = fH * 3;
    MovePen(x, y);
    DrawTextString("字体系列：");
    SetPenColor("Dark Gray");
    MovePen(x + w, y);
    DrawTextString("控制字体系列。建议使用等宽字体。");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - fD * 2 - fH * 1.1, tw, th, fontFamily, sizeof(fontFamily) / sizeof(fontFamily[0]))) {

    }
    tmpStyle.fontFamily = CopyString(fontFamily);

    MovePen(x, y - h);
    DrawTextString("字体大小：");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h);
    DrawTextString("控制字体大小（像素）。请输入正整数。");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - h - fD * 2 - fH * 1.1, tw, th, fontSize, sizeof(fontSize) / sizeof(fontSize[0]))) {

    }
    int tmpFontSize;
    sscanf(fontSize, "%d", &tmpFontSize);
    if (tmpFontSize > 0) {
        tmpStyle.fontSize = tmpFontSize;
    } else if (strlen(fontSize)) {
        SetPenColor("Red");
        MovePen(x + w + tw + fH, y - h - fH - fD);
        DrawTextString("输入必须为正整数");
        SetPenColor(originColor);
    }

    MovePen(x, y - h * 2);
    DrawTextString("行间距：");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h * 2);
    DrawTextString("控制行间距（倍数）。请输入正浮点数。");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - h * 2 - fD * 2 - fH * 1.1, tw, th, lineSpacing, sizeof(lineSpacing) / sizeof(lineSpacing[0]))) {

    }
    double tmpLineSpacing;
    sscanf(lineSpacing, "%lf", &tmpLineSpacing);
    if (tmpLineSpacing > 0) {
        tmpStyle.lineSpacing = tmpLineSpacing;
    } else if (strlen(lineSpacing)) {
        SetPenColor("Red");
        MovePen(x + w + tw + fH, y - h * 2 - fH - fD);
        DrawTextString("输入必须为正浮点数");
        SetPenColor(originColor);
    }

    MovePen(x, y - h * 3);
    DrawTextString("文字颜色：");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h * 3);
    DrawTextString("控制字体颜色。请输入十六进制颜色码，如绿色为 #00FF00。");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - h * 3 - fD * 2 - fH * 1.1, tw, th, textColor, sizeof(textColor) / sizeof(textColor[0]))) {

    }
    if (strlen(textColor)) {        
        if (defineColorRGB("tmpTextColor", textColor)) {
            SetPenColor("tmpTextColor");
            drawRectangle(x + w + tw + fH, y - h * 3 - fD * 2 - fH * 1.1, th, th, 1);
            SetPenColor("Menu Hot Gray");
            drawRectangle(x + w + tw + fH, y - h * 3 - fD * 2 - fH * 1.1, th, th, 0);
            SetPenColor(originColor);
            tmpStyle.textColor = CopyString(textColor);
        } else {
            SetPenColor("Red");
            MovePen(x + w + tw + fH, y - h * 3 - fH - fD);
            DrawTextString("无效的十六进制颜色码");
            SetPenColor(originColor);
            defineColorRGB("tmpTextColor", tmpStyle.textColor);
        }
    } else {
        defineColorRGB("tmpTextColor", tmpStyle.textColor);
        SetPenColor("tmpTextColor");
        drawRectangle(x + w + tw + fH, y - h * 3 - fD * 2 - fH * 1.1, th, th, 1);
        SetPenColor("Menu Hot Gray");
        drawRectangle(x + w + tw + fH, y - h * 3 - fD * 2 - fH * 1.1, th, th, 0);
        SetPenColor(originColor);
    }

    MovePen(x, y - h * 4);
    DrawTextString("背景颜色：");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h * 4);
    DrawTextString("控制背景颜色。请输入十六进制颜色码，如绿色为 #00FF00。");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - h * 4 - fD * 2 - fH * 1.1, tw, th, backgroundColor, sizeof(backgroundColor) / sizeof(backgroundColor[0]))) {

    }
    if (strlen(backgroundColor)) {        
        if (defineColorRGB("tmpBackgroundColor", backgroundColor)) {
            SetPenColor("tmpBackgroundColor");
            drawRectangle(x + w + tw + fH, y - h * 4 - fD * 2 - fH * 1.1, th, th, 1);
            SetPenColor("Menu Hot Gray");
            drawRectangle(x + w + tw + fH, y - h * 4 - fD * 2 - fH * 1.1, th, th, 0);
            SetPenColor(originColor);
            tmpStyle.backgroundColor = CopyString(backgroundColor);
        } else {
            SetPenColor("Red");
            MovePen(x + w + tw + fH, y - h * 4 - fH - fD);
            DrawTextString("无效的十六进制颜色码");
            SetPenColor(originColor);
            defineColorRGB("tmpBackgroundColor", tmpStyle.backgroundColor);
        }
    } else {
        defineColorRGB("tmpBackgroundColor", tmpStyle.backgroundColor);
        SetPenColor("tmpBackgroundColor");
        drawRectangle(x + w + tw + fH, y - h * 4 - fD * 2 - fH * 1.1, th, th, 1);
        SetPenColor("Menu Hot Gray");
        drawRectangle(x + w + tw + fH, y - h * 4 - fD * 2 - fH * 1.1, th, th, 0);
        SetPenColor(originColor);
    }

    if (button(GenUIID(0), x + w + TextStringWidth("控制字体颜色。请输入十六进制颜色码，如红色为 #00FF00。"), y - h * 3 - fD, 
        TextStringWidth("查询颜色码") * 1.1, th, "查询颜色码")) {
        WinExec("cmd.exe /k start https://color.adobe.com/zh/create/color-wheel", SW_HIDE);
    }
    if (button(GenUIID(0), x + w + TextStringWidth("控制字体颜色。请输入十六进制颜色码，如红色为 #00FF00。"), y - h * 4 - fD, 
        TextStringWidth("查询颜色码") * 1.1, th, "查询颜色码")) {
        WinExec("cmd.exe /k start https://color.adobe.com/zh/create/color-wheel", SW_HIDE);
    }

    double H = h;
    y = y - h * 5;
    SetFont(tmpStyle.fontFamily);
    SetPointSize(tmpStyle.fontSize);
    fH = GetFontHeight();
    h = fH * tmpStyle.lineSpacing;
    H = max(H, h * 5);
    char *text[] = {
        "这是一段实例文本",
        "This is a demo text style",
        "Oo0Oo0Oo0Oo0Oo0",
        "Il1Il1Il1Il1Il1"
    };

    w = winWidth / 5;
    for (int i = 0; i < 4; i++) {
        w = max(w, TextStringWidth(text[i])) * 1.2;
    }

    SetPenColor("tmpBackgroundColor");
    drawRectangle(x, y - H, w, H, 1);
    SetPenColor("Menu Hot Gray");
    drawRectangle(x, y - H, w, H, 0);
    fD = GetFontDescent();
    SetPenColor("tmpTextColor");
    for (int i = 0; i < 4; i++) {
        MovePen(x + w * 0.1, y - h * i - fH - fD);
        DrawTextString(text[i]);
    }
    
    SetFont("微软雅黑");
    SetPointSize(13);

    fH = GetFontHeight();
    x = 0;
    y = winHeight;
    h = fH * 1.5;

    SetPointSize(24);
    if (button(GenUIID(0), x + TextStringWidth("  "), y - h, TextStringWidth("←"), h, "←")) {
        isShowSetting ^= 1;
        initSetting ^= 1;
        setTextStyle(tmpStyle);
        SetFont(originFont);
        SetPointSize(originPointSize);
        SetPenColor(originColor);
        return;
    }

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}
/**
 * 绘制键盘快捷键页面
 */ 
static void drawKeyboardPage() {
    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double x = 0;
    double y = winHeight;
    double h = fH * 1.5;

    char *originColor = GetPenColor();
    SetPenColor("Menu Gray");
    drawRectangle(x, y - h, winWidth, h, 1);
    SetPenColor(originColor);

    setButtonColors("Menu Gray", "Black", "Menu Hot Gray", "Black", 1);


    SetPointSize(24);
    if (button(GenUIID(0), x + TextStringWidth("  "), y - h, TextStringWidth("←"), h, "←")) {
        isShowKeyboard ^= 1;
    }

    SetPointSize(40);
    MovePen(winWidth / 16, winHeight * 6 / 7);
    DrawTextString("键盘快捷键");

    x = winWidth / 5;
    y = winHeight * 3 / 4;
    SetPointSize(19);
    fH = GetFontHeight();
    double fD = GetFontDescent();
    h = GetFontHeight() * 1.2;

    static char *tableContent[][2] = {
        "命令", "键绑定",
        "打开文件", "Ctrl + O",
        "保存文件", "Ctrl + S",
        "另存为", "Ctrl + Shift + S",
        "退出Notepad--", "Ctrl + W",
        "编辑：撤销", "Ctrl + Z",
        "编辑：重做", "Ctrl + Y / Ctrl + Shift + Z",
        "编辑：剪切", "Ctrl + X",
        "编辑：复制", "Ctrl + C",
        "编辑：粘贴", "Ctrl + V",
        "编辑：查找", "Ctrl + F",
        "编辑：替换", "Ctrl + H",
        "编辑：全选", "Ctrl + A",
        "打开/保存并退出设置", "Ctrl + E",
        "打开/关闭键盘快捷键界面", "Ctrl + K",
        "帮助", "F1"
    };

    for (int i = 0; i < sizeof(tableContent) / sizeof(tableContent[0]); i++) {
        if (i % 2 == 0) {
            if (i) SetPenColor("Table Gray");
            else SetPenColor("Button Gray");
            drawRectangle(winWidth / 16, y - fD - fH * 0.1 - h * i, winWidth * 14 / 16, h, 1);
            SetPenColor("Dark Gray");
        }
        MovePen(x, y - h * i);
        DrawTextString(tableContent[i][0]);
        MovePen(x + winWidth * 2 / 5, y - h * i);
        DrawTextString(tableContent[i][1]);
    }
    

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}

/**
 * 绘制关于页面
 */
static void drawAboutPage() {
    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double x = 0;
    double y = winHeight;
    double h = fH * 1.5;

    char *originColor = GetPenColor();
    SetPenColor("Menu Gray");
    drawRectangle(x, y - h, winWidth, h, 1);
    SetPenColor(originColor);

    setButtonColors("Menu Gray", "Black", "Menu Hot Gray", "Black", 1);


    SetPointSize(24);
    if (button(GenUIID(0), x + TextStringWidth("  "), y - h, TextStringWidth("←"), h, "←")) {
        isShowAbout ^= 1;
    }

    SetPointSize(40);
    MovePen(winWidth / 16, winHeight * 6 / 7);
    DrawTextString("关于 Notepad--");

    SetPointSize(20);
    fH = GetFontHeight();
    double fD = GetFontDescent();
    h = GetFontHeight() * 1.5;
    x = winWidth * 3 / 32;
    y = winHeight * 6 / 7 - h * 2;

    MovePen(x, y);
    DrawTextString("Notepad-- 由杨沛山、詹含蓓、周健共同开发。");
    MovePen(x, y - h);
    DrawTextString("此应用使用第三方图形库 libgraphics 以及浙江大学C程课程组开发的 simpleGUI 图形库。");
    MovePen(x, y - h * 2);
    DrawTextString("我们对所用的第三方图形库进行了些许修改，并且也用到了 Windows GDI。");
    MovePen(x, y - h * 3);
    DrawTextString("该应用已在 Github 上开源。");
    setButtonColors("White", "Link Blue", "Button Gray", "Link Hot Blue", 1);
    if (button(GenUIID(0), x, y - h * 4 - fD, TextStringWidth("了解更多") * 1.3, fH * 1.2, "了解更多")) {
        WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
    }

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}

/**
 * 绘制右键菜单
 */ 
static void drawContextMenu() {
    
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    static char *contextMenuList[] = {
        "撤销                           Ctrl+Z",
        "重做                           Ctrl+Y",
        "剪切                           Ctrl+X",
        "复制                           Ctrl+C",
        "粘贴                           Ctrl+V",
        "全选                           Ctrl+A"
    };

    double fH = GetFontHeight();
    double h = fH * 1.8;
    double w = TextStringWidth(contextMenuList[0]) * 1.2;
    double x = min(contextMenuX, winWidth - w);
    double y = max(contextMenuY, h * (sizeof(contextMenuList) / sizeof(contextMenuList[0])));

    for (int i = 0; i < sizeof(contextMenuList) / sizeof(contextMenuList[0]); i++) {
        if (button(GenUIID(i), x, y - h * (i + 1), w, h, contextMenuList[i])) {
            switch (i)
            {
            case 0:     //撤销
                undo();
                break;

            case 1:     //重做
                redo();
                break;
            
            case 2:     //剪切
                shearText();
                break;

            case 3:     //复制
                copyText();
                break;

            case 4:     //粘贴
                pasteText();
                break;
            
            case 5:{    //全选
                    int totr = getTotalRow();
                    int totc = getRowLength(totr) + 1;
                    setSelectStartRC((RCNode) {1, 1});
                    setSelectEndRC((RCNode) {totr, totc});
                    setCursorRC((RCNode) {totr, totc});
                    break;
                }
            }
            isShowContextMenu = 0;
        }
    }

    SetFont(originFont);
    SetPointSize(originPointSize);
}

/**
 * 若光标不在窗口中，则移动窗口使光标在窗口中
 */ 
void setCursorInWindow() {
    RCNode cursor = getCursorRC();
    RCNode winCurrent = getWindowCurrentRC();

    char *originFont = GetFont();
    int originPointSize = GetPointSize();

    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();

    double minY = fH * 1.4;
    double menuBarH = fH * 1.5;

    TextStyle style = getTextStyle();
    SetFont(style.fontFamily);
    SetPointSize(style.fontSize);
    fH = GetFontHeight();
    double h = fH * style.lineSpacing;
    double ox = menuBarH / 1.5 * 2 / 3;
    double oy = winHeight - menuBarH - h * 1.25;
    int totl = ceil((oy + GetFontAscent() - minY) / h);     //窗口中显示的总行数
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    if (winCurrent.row > cursor.row - 1) {  //光标在窗口上方
        winCurrent.row = cursor.row;
    } else {    //光标在窗口下方
        if (winCurrent.row + totl - 2 < cursor.row + 1) {
            winCurrent.row = cursor.row - totl + 2;
        }
    }
    
    string s = getRowContent(cursor.row);
    int lens = getRowLength(cursor.row);
    if (lens && s[lens - 1] == '\n') lens--;
    double tx = 0;
    int i = 0, j = 0, lastj = 0;
    while (i < lens) {
        if (s[i] == '\t') {     //处理制表符
            i++;
            if (j % 4 == 0) {
                for (int _i = 1; _i <= 4; _i++) {
                    t[j] = ' ';
                    j++;
                }
                t[j] = t[j + 1] = 0;
            } else {
                while (j % 4) {
                    t[j] = ' ';
                    j++;
                }
                t[j] = t[j + 1] = 0;
            }
        } else {
            if (s[i] & 0x80) {
                t[j] = s[i];
                i++;
                j++;
                t[j] = t[j + 1] = 0;
            }
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        tx += TextStringWidth(t + lastj);
        lastj = j;
        if (cursor.column == i || ox - dx + tx > winWidth - ox) {
            break;
        }
    }
    if (ox - dx + tx < ox) {    //光标在窗口左方
        winCurrent.column = max(winCurrent.column - ceil((dx - tx) / TextStringWidth(" ")), 1);
    } else {
        if (ox - dx + tx > winWidth - ox) {     //光标在窗口右方
            winCurrent.column = winCurrent.column + ceil((ox - dx + tx - winWidth + ox) / TextStringWidth(" "));
        }
    }

    setWindowCurrentRC(winCurrent);

    SetFont(originFont);
    SetPointSize(originPointSize);
}

/**
 * 将窗口的(x, y)像素坐标转换为行列坐标
 */ 
RCNode XYtoRC(int x, int y) {
    
    if (getTotalRow() == 1 && getRowLength(1) == 0) {
        return (RCNode) {1, 1};
    }

    double nx = ScaleXInches(x);
    double ny = ScaleYInches(y);

    RCNode winCurrent = getWindowCurrentRC();
    TextStyle style = getTextStyle();

    int originPointSize = GetPointSize();
    char *originFont = GetFont();

    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double menuBarH = GetFontHeight() * 1.5;
    double minY = fH * 1.4;

    SetFont(style.fontFamily);
    SetPointSize(style.fontSize);
    
    fH = GetFontHeight();
    double ox = menuBarH / 1.5 * 2 / 3;
    double th = fH * style.lineSpacing;
    double oy = winHeight - menuBarH - th * 1.25;
    int totl = ceil((oy + GetFontAscent() - minY) / th);

    RCNode mouse = (RCNode) {0, 0};

    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    for (int row = winCurrent.row - 1; row < min(winCurrent.row + totl, getTotalRow() + 1); row++) {    //row:当前显示的行
        if (oy - th * (row - winCurrent.row) - GetFontDescent() <= ny && ny <= oy + th * 2 - GetFontDescent()) {
            mouse.row = row;
        } else continue;
        string s = getRowContent(row);
        if (s == NULL) continue;
        int i = 0, j = 0, lastj = 0, lens = getRowLength(row);
        double w = 0;
        if (s[lens - 1] == '\n') lens--;
        while (i < lens) {      //i:在s中的下标 j:在t中的下标位置
            if (s[i] == '\t') {     //处理制表符
                i++;
                if (j % 4 == 0) {
                    for (int _i = 1; _i <= 4; _i++) {
                        t[j] = ' ';
                        j++;
                    }
                    t[j] = t[j + 1] = 0;
                } else {
                    while (j % 4) {
                        t[j] = ' ';
                        j++;
                    }
                    t[j] = t[j + 1] = 0;
                }
            } else {
                if (s[i] & 0x80) {
                    t[j] = s[i];
                    i++;
                    j++;
                    t[j] = t[j + 1] = 0;
                }
                t[j] = s[i];
                i++;
                j++;
                t[j] = t[j + 1] = 0;
            }
            w += TextStringWidth(t + lastj);
            lastj = j;
            if (mouse.row) {
                if (nx <= ox - dx + w) {
                    if (i >= 1 && s[i - 1] & 0x80) mouse.column = i - 1;      //处理汉字
                    else mouse.column = i;
                    break;
                }
            }
        }
        if (mouse.row) {
            if (!mouse.column) mouse.column = i + 1;
            break;
        }
    }

    if (!mouse.row) {   //光标在文章末尾下面
        mouse.row = getTotalRow();
        mouse.column = getRowLength(mouse.row) + 1;
    }

    // printf("MOUSE:%d %d\n", mouse.row, mouse.column);

    SetPointSize(originPointSize);
    SetFont(originFont);
    return mouse;
}

/**
 * 设置右键菜单的左上角位置
 */ 
void setContextMenuXY(double x, double y) {
    contextMenuX = x;
    contextMenuY = y;
} 


/**
 * 用于处理键盘按下时的快捷键
 * key      按下的按键
 * isShift  是否按下Shift
 * isCtrl   是否按下Ctrl
 * isTyping 是否在输入状态
 */ 
void processShortcutKey(int key, int isShift, int isCtrl, int isTyping) {
    if (isCtrl && getTextDisplayState()) {  //在文本编辑窗口的快捷键
        switch (key) {
            case 'N':   //Ctrl + N  新建
                createFile();
                break;
            case 'O':   //Ctrl + O  打开
                openFile();
                break;
            case 'S':   
                if (isShift) {
                    saveAnother();  //Ctrl + Shift + S  另存为
                } else {
                    saveFile(); //Ctrl + S  保存
                }
                break;
            case 'W':   //Ctrl + W  退出
                exitApplication();
                break;
            case 'Z':   
                if (isShift) {
                    redo(); //Ctrl + Shift + Z  重做
                } else {
                    undo(); //Ctrl + Z  撤销
                }
                break;
            case 'Y':   //Ctrl + Y  重做
                redo();
                break;
            case 'X':   //Ctrl + X  剪切
                shearText();
                break;
            case 'C':   //Ctrl + C  复制
                copyText();
                break;
            case 'V':   //Ctrl + V  粘贴
                pasteText();
                break;
            case 'F':   //Ctrl + F  查找
                isShowFind ^= 1;
                break;
            case 'H':   //Ctrl + H  替换
                isShowReplace ^= 1;
                break;
            case 'A':   //Ctrl + A  全选
            {
                int totr = getTotalRow();
                int totc = getRowLength(totr) + 1;
                setSelectStartRC((RCNode) {1, 1});
                setSelectEndRC((RCNode) {totr, totc});
                setCursorRC((RCNode) {totr, totc});
                break;
            }
            case 'E':   //Ctrl + E  设置界面
                isShowSetting = 1;
                break;
            case 'K':   //Ctrl + K  键盘快捷键界面
                isShowKeyboard = 1;
                break;
        }
    } else if (isCtrl && isShowSetting) { //设置界面
        switch (key) {
            case 'E':   //Ctrl + E  设置界面
            case 'K':   //Ctrl + K  键盘快捷键界面
                isShowSetting = 0;
                initSetting ^= 1;
                setTextStyle(tmpStyle);
                if (key == 'K') isShowKeyboard = 1;
                break;
            case 'W':   //Ctrl + W  退出
                exitApplication();
                break;
        }
    } else if (isCtrl && isShowKeyboard) {    //键盘快捷键界面
        switch (key) {
            case 'E':   //Ctrl + E  设置界面
            case 'K':   //Ctrl + K  键盘快捷键界面
                isShowKeyboard = 0;
                if (key == 'E') isShowSetting = 1;
                break;
            case 'W':   //Ctrl + W  退出
                exitApplication();
                break;
        }
    } else if (isCtrl && isShowAbout) {   //关于界面
        switch (key) {
            case 'E':   //Ctrl + E  设置界面
                isShowAbout = 0;
                isShowSetting = 1;
                break;
            case 'K':   //Ctrl + K  键盘快捷键界面
                isShowAbout = 0;
                isShowSetting = 1;
                break;
            case 'W':   //Ctrl + W  退出
                exitApplication();
                break;
        }
    }
    if (key == VK_F1) {     //F1    帮助
        WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
    }
}

/**
 * 显示窗口内容
 */ 
void display() {
    if (lockDisplay) {  //如果有display还没结束，则跳过
        return;
    }

    lockDisplay = 1;    //上锁

    DisplayClear();

    winHeight = GetWindowHeight();
	winWidth = GetWindowWidth();
    updateTotalDisplayRow();
    
    if (isShowSetting) {
        drawSettingPage();
    } else if (isShowKeyboard) {
        drawKeyboardPage();
    } else if (isShowAbout) {
        drawAboutPage();
    } else {
        drawTextArea();
        if (isShowFind) {
            drawFindArea();
        }
        if (isShowReplace) {
            drawReplaceArea();
        }
        drawMenu();
        if (isShowContextMenu) {
            drawContextMenu();
        } 
    }

    lockDisplay = 0;    //解锁

}