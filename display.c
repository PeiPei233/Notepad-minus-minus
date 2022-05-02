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

#include <string.h>

static int isShowFind = 0;
static int isShowReplace = 0;


static char inputFindText[10010] = "";
static char inputReplaceText[10010] = "";

void display() {

    DisplayClear();
    // drawSelectRange(1);
    drawTextArea();
    // drawCursor(1);
    if (isShowFind) {
        drawFindArea();
    }
    if (isShowReplace) {
        drawReplaceArea();
    }
    drawMenu();
}

/*
    ���˵� �����ݲ˵�ѡ��ִ�ж�Ӧ�ĺ���������
    ��û�д��κ��ļ�ʱ��ʾ�½��ļ�
*/
void drawMenu() {

    char *originFont = GetFont();
    SetFont("΢���ź�");

    char *menuListFile[] = {"�ļ�",
        "�½�               Ctrl-N",
        "��               Ctrl-O",
        "����               Ctrl-S",
        "�˳�               Ctrl-W"
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
        "����               Ctrl-X",
        "����               Ctrl-C",
        "ճ��               Ctrl-V",
        "����               Ctrl-F",
        "�滻               Ctrl-H"        
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
    }

    SetPenColor("Menu Gray");
    drawRectangle(0, 0, winWidth, fH * 1.4, 1);
    char *s[110];
    RCNode cursor = getCursorRC();
    sprintf(s, "�� %d���� %d", cursor.row, cursor.column);
    SetPenColor("Black");
    MovePen(TextStringWidth("    "), GetFontDescent() + fH * 0.2);
    DrawTextString(s);

    SetPenColor(originColor);
    SetFont(originFont);
}

static char t[10010];

/*
    �ڷ�Χ�ڻ��ı�������Ĳ����������Ͻ�����(x, y) ���εĳ���(w, h) �и� �Ƿ���Ҫ�Զ����� �ȵ�
*/
void drawTextArea() {
    char *s = getCurrentString();
    RCNode winCurrent = getWindowCurrentRC();
    RCNode cursor = getCursorRC();
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();

    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
        RCNode t = startSelect;
        startSelect = endSelect;
        endSelect = t;
    }

    int originPointSize = GetPointSize();
    char *originColor = GetPenColor();
    SetPointSize(15);

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
        cntl++;
        t[0] = t[1] = 0;
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        //draw select area

        // if (s[i] == '\n') {
        //     if (cntl != endSelect.row) {
        //         t[j] = ' ';
        //         i++;
        //         j++;
        //         t[j] = t[j + 1] = 0;
        //     } else i++;
        // }

        SetPenColor("Select Blue");
        if (winCurrent.row == 1) {
            if (cntl > startSelect.row && cntl < endSelect.row) {
                double w = TextStringWidth(t);
                if (w != 0) drawRectangle(x - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
            } else if (cntl == startSelect.row && startSelect.row != endSelect.row) {
                // printf("SELECT:%d %d %s\n", startSelect.row, startSelect.column, t + startSelect.column - 1);
                double w = TextStringWidth(t + startSelect.column - 1);
                char ch = t[startSelect.column - 1];
                t[startSelect.column - 1] = 0;
                if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
                t[startSelect.column - 1] = ch;
            } else if (cntl == endSelect.row && startSelect.row != endSelect.row) {
                char ch = t[endSelect.column - 1];
                t[endSelect.column - 1] = 0;
                double w = TextStringWidth(t);
                if (w != 0) drawRectangle(x - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
                t[endSelect.column - 1] = ch;
            } else if (cntl == startSelect.row && startSelect.row == endSelect.row) {
                char ch1 = t[endSelect.column - 1], ch2 = t[startSelect.column - 1];
                t[endSelect.column - 1] = 0;
                double w = TextStringWidth(t + startSelect.column - 1);
                t[startSelect.column - 1] = 0;
                if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (cntl - 1) - GetFontDescent(), w, th, 1);
                t[endSelect.column - 1] = ch1;
                t[startSelect.column - 1] = ch2;
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
                    char ch = t[startSelect.column - 1];
                    t[startSelect.column - 1] = 0;
                    if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                    t[startSelect.column - 1] = ch;
                } else if (cntl == endSelect.row && startSelect.row != endSelect.row) {
                    char ch = t[endSelect.column - 1];
                    t[endSelect.column - 1] = 0;
                    double w = TextStringWidth(t);
                    if (w != 0) drawRectangle(x - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                    t[endSelect.column - 1] = ch;
                } else if (cntl == startSelect.row && startSelect.row == endSelect.row) {
                    char ch1 = t[endSelect.column - 1], ch2 = t[startSelect.column - 1];
                    t[endSelect.column - 1] = 0;
                    double w = TextStringWidth(t + startSelect.column - 1);
                    t[startSelect.column - 1] = 0;
                    if (w != 0) drawRectangle(x + TextStringWidth(t) - dx, y - th * (k - 2) - GetFontDescent(), w, th, 1);
                    t[endSelect.column - 1] = ch1;
                    t[startSelect.column - 1] = ch2;
                }
            }

        }

        //draw text
        SetPenColor("Black");
        if (winCurrent.row == 1) {
            MovePen(x - dx, y - th * (cntl - 1));
            DrawTextString(t);
        } else if (cntl >= winCurrent.row - 1) {
            MovePen(x - dx, y - th * (k - 2));
            DrawTextString(t);
        }

        //draw cursor
        if (clock() % 1000 < 500) {
            int originPenSize = GetPenSize();
            SetPenSize(2);
            if (winCurrent.row == 1) {
                if (cntl == cursor.row) {
                    t[cursor.column - 1] = 0;
                    MovePen(x + TextStringWidth(t) - dx, y - th * (cntl - 1) - GetFontDescent());
                    DrawLine(0, th);
                }
            } else if (cntl >= winCurrent.row - 1) {
                if (cntl == cursor.row) {
                    t[cursor.column - 1] = 0;
                    MovePen(x + TextStringWidth(t) - dx, y - th * (k - 2) - GetFontDescent());
                    DrawLine(0, th);
                }
            }
            SetPenSize(originPenSize);
        }
        
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }

    SetPointSize(originPointSize);
    SetPenColor(originColor);
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