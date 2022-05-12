/*
    ��Ҫ�����ı��Ļ��� ������ �����滻���ڵĻ��� ��ʽ����ѡ��Ļ���
*/

#include "display.h"
#include "libgraphics.h"
#include "imgui.h"
#include "global.h"
#include "file.h"
#include "edit.h"
#include "find.h"
#include "time.h"
#include "init.h"
#include <windows.h>
#include <string.h>

static int isShowFind = 0;
static int isShowReplace = 0;
static int isShowSetting = 0;
static int isShowKeyboard = 0;
static int isShowAbout = 0;

static char inputFindText[10010] = "";
static char inputReplaceText[10010] = "";

/**
 * ��ȡ���Ҵ�����ʾ״̬
 */ 
int getFindDisplayState() {
    return isShowFind;
}

/**
 * ��ȡ�滻������ʾ״̬
 */ 
int getReplaceDisplayState() {
    return isShowReplace;
}

/**
 * ���Ĳ��Ҵ�����ʾ״̬
 */ 
void setFindDisplayState(int newFindDisplayState) {
    isShowFind = newFindDisplayState;
}

/**
 * �����滻������ʾ״̬
 */ 
void setReplaceDisplayState(int newReplaceDisplayState) {
    isShowReplace = newReplaceDisplayState;
}

/**
 * ��ȡ�ı�������ʾ״̬
 */
int getTextDisplayState() {
    return !(isShowAbout || isShowSetting || isShowKeyboard);
}

void display() {

    DisplayClear();

    // winHeight = GetWindowHeight();
	// winWidth = GetWindowWidth();
    
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
    }
}

/*
    ���˵� �����ݲ˵�ѡ��ִ�ж�Ӧ�ĺ���������
    ��û�д��κ��ļ�ʱ��ʾ�½��ļ�
*/
void drawMenu() {

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
    SetPointSize(13);

    char *menuListFile[] = {"�ļ�",
        "�½�                  Ctrl-N",
        "��                  Ctrl-O",
        "����                  Ctrl-S",
        "�˳�                  Ctrl-W"
    };

    double fH = GetFontHeight();
    double x = 0;
    double y = winHeight;
    double h = fH * 1.5;
    double w = TextStringWidth(menuListFile[0]) * 2;
    double wlist = TextStringWidth(menuListFile[3]) * 1.2;

    char *originColor = GetPenColor();
    SetPenColor("Menu Gray");
    drawRectangle(x, y - h, winWidth, h, 1);
    SetPenColor(originColor);

    int selection;
    selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListFile, sizeof(menuListFile) / sizeof(menuListFile[0]));
    switch (selection) {
        case 1:     //�½�
            createFile();
            break;
        case 2:     //��
            openFile();
            break;
        case 3:     //����
            saveFile();
            break;
        case 4:
            ExitGraphics();
            break;
    }

    char *menuListEdit[] = {"�༭",
        "����                   Ctrl-X",
        "����                   Ctrl-C",
        "ճ��                   Ctrl-V",
        "����                   Ctrl-F",
        "�滻                   Ctrl-H",
        "ȫѡ                   Ctrl-A"        
    };

    selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
    switch (selection) {
        case 1:     //����
            shearText();
            break;
        case 2:     //����
            copyText();
            break;
        case 3:     //ճ��
            pasteText();
            break;
        case 4:     //����
            isShowFind ^= 1;
            break;
        case 5:     //�滻
            isShowReplace ^= 1;
            break;
        case 6:     //ȫѡ
        {
            int totr = getTotalRow();
            int totc = getRowLen(totr);
            setSelectStartRC((RCNode) {1, 1});
            setSelectEndRC((RCNode) {totr, totc});
            setCursorRC((RCNode) {totr, totc});
            break;
        }
    }

    char *menuListSetting[] = {"��ѡ��",
        "����                   Ctrl-E",
        "���̿�ݷ�ʽ      Ctrl-K",
        "���� Notepad--",
        "����"
    };

    selection = menuList(GenUIID(0), x + w * 2, y - h, w, wlist, h, menuListSetting, sizeof(menuListSetting) / sizeof(menuListSetting[0]));
    switch (selection) {
        case 1:     //����
            isShowSetting ^= 1;
            break;
        case 2:     //���̿�ݷ�ʽ
            isShowKeyboard ^= 1;
            break;
        case 3:     //����
            isShowAbout ^= 1;
            break;
        case 4:     //����
            // system("start https://github.com/PeiPei233/UnableToCount");
            WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
            break;
    }

    //״̬������
    SetPenColor("Menu Gray");
    drawRectangle(0, 0, winWidth, fH * 1.4, 1);
    char *s[110];
    RCNode cursor = getCursorRC();
    sprintf(s, "�� %d���� %d", cursor.row, cursor.column);
    SetPenColor("Black");
    MovePen(TextStringWidth("    "), GetFontDescent() + fH * 0.2);
    DrawTextString(s);

    MovePen(winWidth - TextStringWidth("                ") - TextStringWidth("GBK"), GetFontDescent() + fH * 0.2);
    DrawTextString("GBK");

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}

static char t[10010];
static int cid[10010];

/*
    �ڷ�Χ�ڻ��ı�������Ĳ����������Ͻ�����(x, y) ���εĳ���(w, h) �и� �Ƿ���Ҫ�Զ����� �ȵ�
*/
void drawTextArea() {
    char *s = getCurrentString();
    RCNode winCurrent = getWindowCurrentRC();
    RCNode cursor = getCursorRC();
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    
    string originFont = GetFont();
    string originColor=  GetPenColor();
    int originPointSize = GetPointSize();
    TextStyle style = getTextStyle();

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }

    SetPointSize(style.fontSize);
    SetFont(style.fontFamily);
    if (!defineColorRGB("Text Color", style.textColor)) {
        DefineColor("Text Color", 0, 0, 0);
    }
    if (!defineColorRGB("Background Color", style.backgroundColor)) {
        DefineColor("Background Color", 1, 1, 1);
    }
    SetPenColor("Background Color");
    drawRectangle(0, 0, winWidth, winHeight, 1);

    double fH = GetFontHeight();
    double x = fH;
    double y = winHeight - fH * 3;
    double th = fH * style.lineSpacing;
    MovePen(x, y);
    int i = 0, j = 0, k = 0, curl = 0, totl = 0, lens = strlen(s);
    
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    i = j = 0;
    while (i < lens) {
        totl++;
        t[0] = t[1] = 0;
        cid[0] = cid[1] = 0;
        while (i < strlen(s) && s[i] != '\n') {
            if (s[i] == '\t') {
                i++;
                cid[k] = j;
                k++;
                // printf("CID[%d]:%d\n", k, cid[k]);
                if (j % 4 == 0) {
                    for (int _i = 1; _i <= 4; _i++) {
                        t[j] = ' ';
                        j++;
                        t[j] = t[j + 1] = 0;
                    }
                } else {
                    while (j % 4) {
                    t[j] = ' ';
                    j++;
                    t[j] = t[j + 1] = 0;
                    }
                }
            } else {
                t[j] = s[i];
                cid[k] = j;
                i++;
                j++;
                k++;
                t[j] = t[j + 1] = 0;
            }
        }
        cid[k] = j;
        //draw select area

        if (s[i] == '\n') {
            if (totl != endSelect.row) {
                t[j] = ' ';
                i++;
                j++;
                t[j] = t[j + 1] = 0;
            } else i++;
        }

        SetPenColor("Select Blue");
        if (winCurrent.row == 1) {
            if (totl > startSelect.row && totl < endSelect.row) {
                double w = TextStringWidth(t);
                if (w != 0) drawRectangle(x - dx, y - th * (totl - 1) - GetFontDescent(), w, th, 1);
            } else if (totl == startSelect.row && startSelect.row != endSelect.row) {
                // printf("SELECT:%d %d %s\n", startSelect.row, startSelect.column, t + startSelect.column - 1);
                double w = TextStringWidth(t + cid[startSelect.column - 1]);
                char ch = t[cid[startSelect.column - 1]];
                t[cid[startSelect.column - 1]] = 0;
                if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (totl - 1) - GetFontDescent(), w, th, 1);
                t[cid[startSelect.column - 1]] = ch;
            } else if (totl == endSelect.row && startSelect.row != endSelect.row) {
                char ch = t[cid[endSelect.column - 1]];
                t[cid[endSelect.column - 1]] = 0;
                double w = TextStringWidth(t);
                if (w != 0) drawRectangle(x - dx, y - th * (totl - 1) - GetFontDescent(), w, th, 1);
                t[cid[endSelect.column - 1]] = ch;
            } else if (totl == startSelect.row && startSelect.row == endSelect.row) {
                char ch1 = t[cid[endSelect.column - 1]], ch2 = t[cid[startSelect.column - 1]];
                t[cid[endSelect.column - 1]] = 0;
                double w = TextStringWidth(t + cid[startSelect.column - 1]);
                t[cid[startSelect.column - 1]] = 0;
                if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (totl - 1) - GetFontDescent(), w, th, 1);
                t[cid[endSelect.column - 1]] = ch1;
                t[cid[startSelect.column - 1]] = ch2;
            }
        } else {
            if (totl >= winCurrent.row - 1) {
                curl++;
                if (totl > startSelect.row && totl < endSelect.row) {
                    double w = TextStringWidth(t);
                    if (w != 0) drawRectangle(x - dx, y - th * (curl - 2) - GetFontDescent(), w, th, 1);
                } else if (totl == startSelect.row && startSelect.row != endSelect.row) {
                    // printf("SELECT:%d %d %s\n", startSelect.row, startSelect.column, t + startSelect.column - 1);
                    double w = TextStringWidth(t + cid[startSelect.column - 1]);
                    char ch = t[cid[startSelect.column - 1]];
                    t[cid[startSelect.column - 1]] = 0;
                    if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (curl - 2) - GetFontDescent(), w, th, 1);
                    t[cid[startSelect.column - 1]] = ch;
                } else if (totl == endSelect.row && startSelect.row != endSelect.row) {
                    char ch = t[cid[endSelect.column - 1]];
                    t[cid[endSelect.column - 1]] = 0;
                    double w = TextStringWidth(t);
                    if (w != 0) drawRectangle(x - dx, y - th * (curl - 2) - GetFontDescent(), w, th, 1);
                    t[cid[endSelect.column - 1]] = ch;
                } else if (totl == startSelect.row && startSelect.row == endSelect.row) {
                    char ch1 = t[cid[endSelect.column - 1]], ch2 = t[cid[startSelect.column - 1]];
                    t[cid[endSelect.column - 1]] = 0;
                    double w = TextStringWidth(t + cid[startSelect.column - 1]);
                    t[cid[startSelect.column - 1]] = 0;
                    if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (curl - 2) - GetFontDescent(), w, th, 1);
                    t[cid[endSelect.column - 1]] = ch1;
                    t[cid[startSelect.column - 1]] = ch2;
                }
            }

        }

        //draw text
        SetPenColor("Text Color");
        if (winCurrent.row == 1) {
            MovePen(x - dx, y - th * (totl - 1));
            DrawTextString(t);
        } else if (totl >= winCurrent.row - 1) {
            MovePen(x - dx, y - th * (curl - 2));
            DrawTextString(t);
        }

        //draw cursor
        if (clock() % 1000 < 500) {
            int originPenSize = GetPenSize();
            SetPenSize(2);
            if (winCurrent.row == 1) {
                if (totl == cursor.row) {
                    t[cid[cursor.column - 1]] = 0;
                    MovePen(x + TextStringWidth(t) - dx, y - th * (totl - 1) - GetFontDescent());
                    DrawLine(0, th);
                }
            } else if (totl >= winCurrent.row - 1) {
                if (totl == cursor.row) {
                    t[cid[cursor.column - 1]] = 0;
                    MovePen(x + TextStringWidth(t) - dx, y - th * (curl - 2) - GetFontDescent());
                    DrawLine(0, th);
                }
            }
            SetPenSize(originPenSize);
        }
        
        j = k = 0;
        t[0] = t[1] = 0;
        // if (s[i] == '\n') i++;
        if (winCurrent.row == 1) {
            if (y - th * totl < 0) break;
        } else if (totl >= winCurrent.row - 1) {
            if (y - th * curl < 0) break;
        }
    }

    SetPointSize(originPointSize);
    SetPenColor(originColor);
    SetFont(originFont);
}

/*
    ���ݴ�������ı��򴰿��е�����λ��(r, c)��ʾ���
    ��Ҫ�Ĳ������������������ı������Ͻ����꣬�иߵȵ�
*/
void drawCursor(double rowHeight) {
    RCNode cursor = getCursorRC();
    RCNode winCurrent = getWindowCurrentRC();

    // printf("CURSOR:%d %d\n", cursor.row, cursor.column);

    if (cursor.row < winCurrent.row - 1) return;

    char *s = getCurrentString();

    int originPointSize = GetPointSize();
    int originPenSize = GetPenSize();
    SetPointSize(15);
    SetPenSize(2);

    double fH = GetFontHeight();
    double x = fH;
    double y = winHeight - fH * 3;
    double w = winWidth - fH * 2;
    double th = fH * 1.2;
    MovePen(x, y);
    int i = 0, j = 0, k = 0, cntl = 0, lens = strlen(s);
    
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    i = j = 0;
    while (i < lens) {
        t[0] = t[1] = 0;
        cntl++;
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        if (winCurrent.row == 1) {
            if (cntl == cursor.row) {
                t[cursor.column - 1] = 0;
                MovePen(x + TextStringWidth(t) - dx, y - th * (cntl - 1) - GetFontDescent());
                DrawLine(0, th);
                i = lens + 1;
                t[0] = t[1] = 0;
                break;
            }
        } else if (cntl >= winCurrent.row - 1) {
            k++;
            if (cntl == cursor.row) {
                t[cursor.column - 1] = 0;
                MovePen(x + TextStringWidth(t) - dx, y - th * (k - 2) - GetFontDescent());
                DrawLine(0, th);
                i = lens + 1;
                t[0] = t[1] = 0;
                break;
            }
        }
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }

    SetPointSize(originPointSize);
    SetPenSize(originPenSize);
}

/*
    ����ѡ�еķ�Χ���Ƹ����ı���ɫ
    ע��һ��Ҫ�Ȼ����ı���ɫ �ٻ����ı�
    ������������������ѡ�з�Χ����ʼ������������ֹ��������
*/
void drawSelectRange(double rowHeight) {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    RCNode winCurrent = getWindowCurrentRC();

    // printf("SELECT FROM %d %d TO %d %d\n", startSelect.row, startSelect.column, endSelect.row, endSelect.column);

    if (startSelect.column == endSelect.column && startSelect.row == endSelect.row) return;

    char *originColor = GetPenColor();
    SetPenColor("Select Blue");

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }

    char *s = getCurrentString();
    int originPointSize = GetPointSize();
    SetPointSize(15);

    double fH = GetFontHeight();
    double x = fH;
    double y = winHeight - fH * 3;
    double w = winWidth - fH * 2;
    double th = fH * 1.2;
    
    int i = 0, j = 0, k = 0, cntl = 0, lens = strlen(s);
    
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    i = j = 0;
    while (i < lens) {
        t[0] = t[1] = 0;
        cntl++;
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        if (s[i] == '\n') {
            if (cntl != endSelect.row) {
                t[j] = ' ';
                i++;
                j++;
                t[j] = t[j + 1] = 0;
            } else i++;
        }
        if (winCurrent.row == 1) {
            if (cntl > startSelect.row && cntl < endSelect.row) {
                double w = TextStringWidth(t);
                if (w != 0) drawRectangle(x - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
            } else if (cntl == startSelect.row && startSelect.row != endSelect.row) {
                // printf("SELECT:%d %d %s\n", startSelect.row, startSelect.column, t + startSelect.column - 1);
                double w = TextStringWidth(t + startSelect.column - 1);
                t[startSelect.column - 1] = 0;
                if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
            } else if (cntl == endSelect.row && startSelect.row != endSelect.row) {
                t[endSelect.column - 1] = 0;
                double w = TextStringWidth(t);
                if (w != 0) drawRectangle(x - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
            } else if (cntl == startSelect.row && startSelect.row == endSelect.row) {
                t[endSelect.column - 1] = 0;
                double w = TextStringWidth(t + startSelect.column - 1);
                t[startSelect.column - 1] = 0;
                if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
            }
        } else {
            if (cntl >= winCurrent.row - 1) {
                k++;
                if (cntl > startSelect.row && cntl < endSelect.row) {
                    double w = TextStringWidth(t);
                    if (w != 0) drawRectangle(x - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                } else if (cntl == startSelect.row && startSelect.row != endSelect.row) {
                    // printf("SELECT:%d %d %s\n", startSelect.row, startSelect.column, t + startSelect.column - 1);
                    double w = TextStringWidth(t + startSelect.column - 1);
                    t[startSelect.column - 1] = 0;
                    if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                } else if (cntl == endSelect.row && startSelect.row != endSelect.row) {
                    t[endSelect.column - 1] = 0;
                    double w = TextStringWidth(t);
                    if (w != 0) drawRectangle(x - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                } else if (cntl == startSelect.row && startSelect.row == endSelect.row) {
                    t[endSelect.column - 1] = 0;
                    double w = TextStringWidth(t + startSelect.column - 1);
                    t[startSelect.column - 1] = 0;
                    if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                }
            }

        }
        j = 0;
        t[0] = t[1] = 0;
    }

    SetPointSize(originPointSize);
    SetPenColor(originColor);
}

/*
    ���Ʋ��Ҵ��� ����һ�����������ı���Ͳ��Ұ�ť��
    ����������ļ��ı������Ͻ�
    �Ȼ��Ƶײ��ı� �ڻ��Ʋ��Ҵ���ʱ�Ȼ���һ����ɫ���θ��ǵ��µ��ı����ڻ��Ʋ��Ҵ���
    �����滻����ͬ��
*/
void drawFindArea() {
    
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);

    char *originFont = GetFont();
    SetFont("΢���ź�");

    double fH = GetFontHeight();
    double x = winWidth / 2;
    double y = winHeight - fH * 3.5;
    double w = x * 15 / 16;
    double h = fH * 1.6;

    char *originColor = GetPenColor();
    SetPenColor("Button Gray");
    drawRectangle(x, y, w, h, 1);
    SetPenColor("Light Gray");
    drawRectangle(x, y, w, h, 0);
    SetPenColor(originColor);

    double buttonFindToReplaceX = x + fH * 0.2;
    double buttonFindToReplaceY = y + fH * 0.2;
    double buttonFindToReplaceW = fH * 1.2;
    double buttonFindToReplaceH = fH * 1.2;

    double textboxFindX = x + fH * 1.6;
    double textboxFindY = buttonFindToReplaceY;
    double textboxFindW = w * 2 / 3;
    double textboxFindH = buttonFindToReplaceH;

    char *findButton = "������һ��";
    double buttonFindX = textboxFindX + textboxFindW + (w - textboxFindW - (textboxFindX - x)) / 12;
    double buttonFindY = textboxFindY;
    double buttonFindW = TextStringWidth(findButton) * 1.1;
    double buttonFindH = textboxFindH;

    double buttonCloseX = x + w - fH * 1.4;
    double buttonCloseY = textboxFindY;
    double buttonCloseW = fH * 1.2;
    double buttonCloseH = textboxFindH;

    SetPenColor("White");
    drawRectangle(textboxFindX, textboxFindY, textboxFindW, textboxFindH, 1);
    SetPenColor(originColor);
    if (textbox(GenUIID(0), textboxFindX, textboxFindY, textboxFindW, textboxFindH, inputFindText, sizeof(inputFindText) / sizeof(inputFindText[0]))) {
        
    }
    
    if (button(GenUIID(0), buttonFindX, buttonFindY, buttonFindW, buttonFindH, findButton)) {
        findText(inputFindText);
    }
    
    if (strlen(inputFindText) < 1) {
        SetPenColor("Dark Gray");
        MovePen(textboxFindX + GetFontAscent() / 2, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        DrawTextString("����");
        SetPenColor(originColor);
    }
    
    if (button(GenUIID(0), buttonFindToReplaceX, buttonFindToReplaceY, buttonFindToReplaceW, buttonFindToReplaceH, "��")) {
        isShowFind = 0;
        isShowReplace = 1;
        SetFont(originFont);
        display();
        return;
    }

    if (button(GenUIID(0), buttonCloseX, buttonCloseY, buttonCloseW, buttonCloseH, "��")) {
        isShowFind = 0;
        SetFont(originFont);
        display();
        return;
    }

    SetFont(originFont);

}

/*
    �����滻���� ���������ı��򣨷ֱ��ǲ��Һ��滻���Լ��滻��ť��
*/
void drawReplaceArea() {

    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);

    char *originFont = GetFont();
    SetFont("΢���ź�");

    double fH = GetFontHeight();
    double x = winWidth / 2;
    double y = winHeight - fH * 4.9;
    double w = x * 15 / 16;
    double h = fH * 3;

    char *originColor = GetPenColor();
    SetPenColor("Button Gray");
    drawRectangle(x, y, w, h, 1);
    SetPenColor("Light Gray");
    drawRectangle(x, y, w, h, 0);
    SetPenColor(originColor);

    double textboxReplaceX = x + fH * 1.6;
    double textboxReplaceY = y + fH * 0.2;
    double textboxReplaceW = w * 2 / 3;
    double textboxReplaceH = fH * 1.2;
    // memset(inputText, 0, sizeof(inputText));

    double buttonReplaceToFindX = x + fH * 0.2;
    double buttonReplaceToFindY = y + fH * 0.2;
    double buttonReplaceToFindW = fH * 1.2;
    double buttonReplaceToFindH = fH * 2.6;

    double textboxFindX = x + fH * 1.6;
    double textboxFindY = y + fH * 0.2 + fH * 1.4;
    double textboxFindW = w * 2 / 3;
    double textboxFindH = fH * 1.2;

    char *findButton = "������һ��";
    double buttonFindX = textboxFindX + textboxFindW + (w - textboxFindW - (textboxFindX - x)) / 12;
    double buttonFindY = textboxFindY;
    double buttonFindW = TextStringWidth(findButton) * 1.1;
    double buttonFindH = textboxFindH;

    double buttonCloseX = x + w - fH * 1.4;
    double buttonCloseY = textboxFindY;
    double buttonCloseW = fH * 1.2;
    double buttonCloseH = textboxFindH;

    char *replaceButton = "�滻";
    double buttonReplaceX = textboxReplaceX + textboxReplaceW + (w - textboxReplaceW - (textboxReplaceX - x)) / 12;
    double buttonReplaceY = textboxReplaceY;
    double buttonReplaceW = TextStringWidth(replaceButton) * 1.1;
    double buttonReplaceH = textboxReplaceH;

    SetPenColor("White");
    drawRectangle(textboxFindX, textboxFindY, textboxFindW, textboxFindH, 1);
    SetPenColor(originColor);
    if (textbox(GenUIID(0), textboxFindX, textboxFindY, textboxFindW, textboxFindH, inputFindText, sizeof(inputFindText) / sizeof(inputFindText[0]))) {
        
    }

    if (button(GenUIID(0), buttonFindX, buttonFindY, buttonFindW, buttonFindH, findButton)) {
        findText(inputFindText);
    }

    if (button(GenUIID(0), buttonReplaceToFindX, buttonReplaceToFindY, buttonReplaceToFindW, buttonReplaceToFindH, "��")) {
        isShowFind = 1;
        isShowReplace = 0;
        SetFont(originFont);
        display();
        return;
    }

    if (button(GenUIID(0), buttonCloseX, buttonCloseY, buttonCloseW, buttonCloseH, "��")) {
        isShowReplace = 0;
        SetFont(originFont);
        display();
        return;
    }

    SetPenColor("White");
    drawRectangle(textboxReplaceX, textboxReplaceY, textboxReplaceW, textboxReplaceH, 1);
    SetPenColor(originColor);
    if (textbox(GenUIID(0), textboxReplaceX, textboxReplaceY, textboxReplaceW, textboxReplaceH, inputReplaceText, sizeof(inputReplaceText) / sizeof(inputReplaceText[0]))) {
        
    }

    if (strlen(inputFindText) < 1) {
        SetPenColor("Dark Gray");
        MovePen(textboxFindX + GetFontAscent() / 2, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        DrawTextString("����");
        SetPenColor(originColor);
    }

    if (strlen(inputReplaceText) < 1) {
        SetPenColor("Dark Gray");
        MovePen(textboxReplaceX + GetFontAscent() / 2, textboxReplaceY + textboxReplaceH / 2 - GetFontAscent() / 2);
        DrawTextString("�滻");
        SetPenColor(originColor);
    }

    if (button(GenUIID(0), buttonReplaceX, buttonReplaceY, buttonReplaceW, buttonReplaceH, replaceButton)) {
        replaceText(inputFindText, inputReplaceText);
    }

    SetFont(originFont);

}

static char fontFamily[10010];
static char fontSize[10010];
static char lineSpacing[10010];
static char backgroundColor[10010];
static char textColor[10010];
static int initSetting = 1;

void drawSettingPage() {
    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
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

    TextStyle style = getTextStyle();

    if (initSetting) {
        initSetting = 0;
        strcpy(fontFamily, style.fontFamily);
        strcpy(fontSize, IntegerToString(style.fontSize));
        strcpy(lineSpacing, RealToString(style.lineSpacing));
        strcpy(backgroundColor, style.backgroundColor);
        strcpy(textColor, style.textColor);
    }

    SetPointSize(40);
    MovePen(winWidth / 16, winHeight * 6 / 7);
    DrawTextString("����");

    x = winWidth / 8;
    y = winHeight * 6 / 8;
    SetPointSize(16);
    double w = TextStringWidth("����ϵ�У�") * 1.1;
    double tw = winWidth / 3;
    fH = GetFontHeight();
    double fD = GetFontDescent();
    double th = fH * 1.2;
    h = fH * 3;
    MovePen(x, y);
    DrawTextString("����ϵ�У�");
    SetPenColor("Dark Gray");
    MovePen(x + w, y);
    DrawTextString("��������ϵ�С�");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - fD * 2 - fH * 1.1, tw, th, fontFamily, sizeof(fontFamily) / sizeof(fontFamily[0]))) {

    }
    style.fontFamily = CopyString(fontFamily);

    MovePen(x, y - h);
    DrawTextString("�����С��");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h);
    DrawTextString("���������С�����أ�����������������");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - h - fD * 2 - fH * 1.1, tw, th, fontSize, sizeof(fontSize) / sizeof(fontSize[0]))) {

    }
    int tmpFontSize;
    sscanf(fontSize, "%d", &tmpFontSize);
    if (tmpFontSize > 0) {
        style.fontSize = tmpFontSize;
    } else if (strlen(fontSize)) {
        SetPenColor("Red");
        MovePen(x + w + tw + fH, y - h - fH - fD);
        DrawTextString("�������Ϊ������");
        SetPenColor(originColor);
    }

    MovePen(x, y - h * 2);
    DrawTextString("�м�ࣺ");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h * 2);
    DrawTextString("�����м�ࣨ������������������������");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - h * 2 - fD * 2 - fH * 1.1, tw, th, lineSpacing, sizeof(lineSpacing) / sizeof(lineSpacing[0]))) {

    }
    double tmpLineSpacing;
    sscanf(lineSpacing, "%lf", &tmpLineSpacing);
    if (tmpLineSpacing > 0) {
        style.lineSpacing = tmpLineSpacing;
    } else if (strlen(lineSpacing)) {
        SetPenColor("Red");
        MovePen(x + w + tw + fH, y - h * 2 - fH - fD);
        DrawTextString("�������Ϊ��������");
        SetPenColor(originColor);
    }

    MovePen(x, y - h * 3);
    DrawTextString("������ɫ��");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h * 3);
    DrawTextString("����������ɫ��������ʮ��������ɫ�룬����ɫΪ #00FF00��");
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
            style.textColor = CopyString(textColor);
        } else {
            SetPenColor("Red");
            MovePen(x + w + tw + fH, y - h * 3 - fH - fD);
            DrawTextString("��Ч��ʮ��������ɫ��");
            SetPenColor(originColor);
            defineColorRGB("tmpTextColor", style.textColor);
        }
    } else {
        defineColorRGB("tmpTextColor", style.textColor);
        SetPenColor("tmpTextColor");
        drawRectangle(x + w + tw + fH, y - h * 3 - fD * 2 - fH * 1.1, th, th, 1);
        SetPenColor("Menu Hot Gray");
        drawRectangle(x + w + tw + fH, y - h * 3 - fD * 2 - fH * 1.1, th, th, 0);
        SetPenColor(originColor);
    }

    MovePen(x, y - h * 4);
    DrawTextString("������ɫ��");
    SetPenColor("Dark Gray");
    MovePen(x + w, y - h * 4);
    DrawTextString("���Ʊ�����ɫ��������ʮ��������ɫ�룬����ɫΪ #00FF00��");
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
            style.backgroundColor = CopyString(backgroundColor);
        } else {
            SetPenColor("Red");
            MovePen(x + w + tw + fH, y - h * 4 - fH - fD);
            DrawTextString("��Ч��ʮ��������ɫ��");
            SetPenColor(originColor);
            defineColorRGB("tmpBackgroundColor", style.backgroundColor);
        }
    } else {
        defineColorRGB("tmpBackgroundColor", style.backgroundColor);
        SetPenColor("tmpBackgroundColor");
        drawRectangle(x + w + tw + fH, y - h * 4 - fD * 2 - fH * 1.1, th, th, 1);
        SetPenColor("Menu Hot Gray");
        drawRectangle(x + w + tw + fH, y - h * 4 - fD * 2 - fH * 1.1, th, th, 0);
        SetPenColor(originColor);
    }

    if (button(GenUIID(0), x + w + TextStringWidth("����������ɫ��������ʮ��������ɫ�룬���ɫΪ #00FF00��"), y - h * 3 - fD, 
        TextStringWidth("��ѯ��ɫ��") * 1.1, th, "��ѯ��ɫ��")) {
        WinExec("cmd.exe /k start https://color.adobe.com/zh/create/color-wheel", SW_HIDE);
    }
    if (button(GenUIID(0), x + w + TextStringWidth("����������ɫ��������ʮ��������ɫ�룬���ɫΪ #00FF00��"), y - h * 4 - fD, 
        TextStringWidth("��ѯ��ɫ��") * 1.1, th, "��ѯ��ɫ��")) {
        WinExec("cmd.exe /k start https://color.adobe.com/zh/create/color-wheel", SW_HIDE);
    }

    double H = h;
    y = y - h * 5;
    SetPointSize(style.fontSize);
    SetFont(style.fontFamily);
    fH = GetFontHeight();
    h = fH * style.lineSpacing;
    H = H > h * 5 ? H : h * 5;
    char *text[] = {
        "����һ��ʵ���ı�",
        "This is a demo text style",
        "Oo0Oo0Oo0Oo0Oo0",
        "Il1Il1Il1Il1Il1"
    };

    w = winWidth / 5;
    for (int i = 0; i < 4; i++) {
        double t = TextStringWidth(text[i]) * 1.2;
        w = w > t ? w : t;
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
    
    SetFont("΢���ź�");
    SetPointSize(13);

    fH = GetFontHeight();
    x = 0;
    y = winHeight;
    h = fH * 1.5;

    SetPointSize(24);
    if (button(GenUIID(0), x + TextStringWidth("  "), y - h, TextStringWidth("��"), h, "��")) {
        isShowSetting ^= 1;
        initSetting ^= 1;
        setTextStyle(style);
        SetFont(originFont);
        SetPointSize(originPointSize);
        SetPenColor(originColor);
        return;
    }

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}

void drawKeyboardPage() {
    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
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
    if (button(GenUIID(0), x + TextStringWidth("  "), y - h, TextStringWidth("��"), h, "��")) {
        isShowKeyboard ^= 1;
    }

    SetPointSize(40);
    MovePen(winWidth / 16, winHeight * 6 / 7);
    DrawTextString("���̿�ݼ�");

    x = winWidth / 5;
    y = winHeight * 3 / 4;
    SetPointSize(20);
    fH = GetFontHeight();
    double fD = GetFontDescent();
    h = GetFontHeight() * 1.2;

    static char *tableContent[][2] = {
        "����", "����",
        "���ļ�", "Ctrl + O",
        "�����ļ�", "Ctrl + S",
        "�˳�Notepad--", "Ctrl + W",
        "����", "Ctrl + X",
        "����", "Ctrl + C",
        "ճ��", "Ctrl + V",
        "����", "Ctrl + F",
        "�滻", "Ctrl + H",
        "ȫѡ", "Ctrl + A",
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

void drawAboutPage() {
    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
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
    if (button(GenUIID(0), x + TextStringWidth("  "), y - h, TextStringWidth("��"), h, "��")) {
        isShowAbout ^= 1;
    }

    SetPointSize(40);
    MovePen(winWidth / 16, winHeight * 6 / 7);
    DrawTextString("���� Notepad--");

    x = winWidth / 8;
    y = winHeight * 3 / 4;
    SetPointSize(20);
    fH = GetFontHeight();
    double fD = GetFontDescent();
    h = GetFontHeight() * 1.2;

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}