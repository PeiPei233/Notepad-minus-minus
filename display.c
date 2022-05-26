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
#include "style.h"
#include "callback.h"
#include "storage.h"
#include "record.h"
#include <math.h>
#include <windows.h>
#include <string.h>

static int isShowFind = 0;          //�Ƿ���ʾ���ҿ�
static int isShowReplace = 0;       //�Ƿ���ʾ�滻��
static int isShowSetting = 0;       //�Ƿ���ʾ����ҳ��
static int isShowKeyboard = 0;      //�Ƿ���ʾ���̿�ݼ�ҳ��
static int isShowAbout = 0;         //�Ƿ���ʾ����ҳ��
static int isShowContextMenu = 0;   //�Ƿ���ʾ�Ҽ��˵�

//�Ҽ��˵�����
static double contextMenuX;
static double contextMenuY;

static int lockDisplay = 0;     //������display��������ֹ��һ����û�����ֿ�ʼdisplay

static char inputFindText[100010] = "";
static char inputReplaceText[100010] = "";

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
 * ��ȡ�Ҽ��˵���ʾ״̬
 */ 
int getContextMenuDisplayState() {
    return isShowContextMenu; 
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
 * �����Ҽ��˵���ʾ״̬
 */ 
void setContextMenuDisplayState(int newContextMenuDisplayState) {
    isShowContextMenu = newContextMenuDisplayState;
}

/**
 * ��ȡ�ı�������ʾ״̬
 */
int getTextDisplayState() {
    return !(isShowAbout || isShowSetting || isShowKeyboard);
}

static int totDisplayRow;

/**
 * ��ȡ��ǰ��ʾ��������
 */ 
int getTotalDisplayRow() {
    return totDisplayRow;
}

/**
 * ���µ�ǰ��ʾ��������
 */ 
void updateTotalDisplayRow() {    
    string originFont = GetFont();
    string originColor=  GetPenColor();
    int originPointSize = GetPointSize();
    TextStyle style = getTextStyle();

    SetFont("΢���ź�");
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
    ���˵� �����ݲ˵�ѡ��ִ�ж�Ӧ�ĺ���������
    ��û�д��κ��ļ�ʱ��ʾ�½��ļ�
*/
static void drawMenu() {

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
    SetPointSize(13);

    char *menuListFile[] = {"�ļ�",
        "�½�                       Ctrl + N",
        "��                       Ctrl + O",
        "����                       Ctrl + S",
        "���Ϊ         Ctrl + Shift + S",
        "�˳�                       Ctrl + W"
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
        case 1:     //�½�
            createFile();
            break;
        case 2:     //��
            openFile();
            break;
        case 3:     //����
            saveFile();
            break;
        case 4:     //����
            saveAnother();
            break;
        case 5:     //�˳�
            exitApplication();
            break;
    }

    char *menuListEdit[] = {"�༭",
        "����                       Ctrl + Z",
        "����                       Ctrl + Y",
        "����                       Ctrl + X",
        "����                       Ctrl + C",
        "ճ��                       Ctrl + V",
        "����                       Ctrl + F",
        "�滻                       Ctrl + H",
        "ȫѡ                       Ctrl + A"
    };

    selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListEdit, sizeof(menuListEdit) / sizeof(menuListEdit[0]));
    switch (selection) {
        case 3:     //����
            shearText();
            break;
        case 4:     //����
            copyText();
            break;
        case 5:     //ճ��
            pasteText();
            break;
        case 6:     //����
            isShowFind ^= 1;
            break;
        case 7:     //�滻
            isShowReplace ^= 1;
            break;
        case 8:     //ȫѡ
        {
            int totr = getTotalRow();
            int totc = getRowLength(totr) + 1;
            setSelectStartRC((RCNode) {1, 1});
            setSelectEndRC((RCNode) {totr, totc});
            setCursorRC((RCNode) {totr, totc});
            break;
        }
        case 1:     //����
            undo();
            break;
        case 2:     //����
            redo();
            break;
    }

    char *menuListSetting[] = {"��ѡ��",
        "����                       Ctrl + E",
        "���̿�ݷ�ʽ          Ctrl + K",
        "���� Notepad--",
        "����                               F1"
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
            WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
            break;
    }

    //�ļ���
    SetPenColor("Black");
    string fileName;
    static char name[10010];
    if (!(fileName = getCurrentFileName())) {
        if (getSaveState()) sprintf(name, "�ޱ��� - Notepad--");
        else sprintf(name, "�ޱ���* - Notepad--");
    } else {
        if (getSaveState()) sprintf(name, "%s - Notepad--", fileName);
        else sprintf(name, "%s* - Notepad--", fileName);
    }
    double tw = TextStringWidth(name);
    double tx = max(winWidth / 2 - tw / 2, x + w * 3.5);
    int i = strlen(name) - 1;
    while (tx + TextStringWidth(name) > winWidth - h * 2 / 3 && i >= 0) {    //���ֹ����������ʾʡ�Ժ�...
        if (name[i] & 0x80) i--;
        i--;
        name[i + 1] = name[i + 2] = name[i + 3] = '.';
        name[i + 4] = 0;
    }
    MovePen(tx, winHeight - h / 2 - GetFontAscent() / 2);
    DrawTextString(name);
    // printf("%s\n", name);

    //״̬������
    SetPenColor("Menu Gray");
    drawRectangle(0, 0, winWidth, fH * 1.4, 1);
    static char s[110];
    RCNode cursor = getCursorRC();
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    if (startSelect.column == endSelect.column && startSelect.row == endSelect.row) {
        sprintf(s, "�� %d���� %d", cursor.row, cursor.column);
        SetPenColor("Black");
        MovePen(TextStringWidth("    "), GetFontDescent() + fH * 0.2);
        DrawTextString(s);
    } else {
        int len = getContentLength(getSelectStartRC(), getSelectEndRC());
        sprintf(s, "�� %d���� %d����ѡ��%d��", cursor.row, cursor.column, len);
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
static char *t;     //ĳһ�е��ַ�
static double *textWidth;   //textWidth[i]: [0, i)������ַ�������

/*
    �����ı�����
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

    SetFont("΢���ź�");
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

    if (getTotalRow() == 1 && getRowLength(1) == 0) {    //���ַ���
        if (clock() % 1000 < 500 && getTypingState()) {
            int originPenSize = GetPenSize();
            SetPenColor("Text Color");
            SetPenSize(2);
            MovePen(x - dx, y - GetFontDescent());
            DrawLine(0, th);
            SetPenSize(originPenSize);
        }
    }

    for (int row = winCurrent.row - 1; row < winCurrent.row + totl; row++) {    //row:��ǰ��ʾ����
        string s = getRowContent(row);
        if (s == NULL) continue;
        int i = 0, j = 0, lastj = 0, lens = getRowLength(row);  //i:ԭ����s���±꣬j:�����Ʊ������������±�
        int starti = -1, endi = -1;   //����ʾ���ֵĿ�ʼ/������s���±�i
        if (lens && s[lens - 1] == '\n') lens--;
        unsigned int cntT = 0;   //������'\t'������
        while (i < lens) {
            if (s[i] == '\t') {
                cntT++;
            }
            i++;
        }
        i = 0;
        if (!capT) {    //û��ʼ���������³�ʼ��
            capT = 1024;
            while (capT < lens + cntT * 4 + 5) {
                capT <<= 1;
            }
            t = (char *) malloc(sizeof(char) * capT);
            textWidth = (double *) malloc(sizeof(double) * capT);
        } 
        else if (capT < lens + cntT * 4 + 5) {  //װ�����ˣ����³�ʼ��
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
        while (i < lens) {      //i:��s�е��±� j:��t�е��±�λ��
            if (s[i] == '\t') {     //�����Ʊ��
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
        if (s[i] == '\n') {     //���ѡ�񲿷���'\n'�����Կո��ʾ����
            if (row != endSelect.row) {
                lens++;
                textWidth[lens] = textWidth[lens - 1] + TextStringWidth(" ");
            }
        }

        SetPenColor("Select Blue");
        if (row > startSelect.row && row < endSelect.row) {     //������ѡ������м�
            double w = textWidth[lens];
            double sx = max(0, x - dx);
            double ex = min(winWidth, x - dx + w);
            if (ex - sx != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        } else if (row == startSelect.row && startSelect.row != endSelect.row) {    //������ѡ��ĵ�һ��
            // printf("SELECT:(%d, %d) TO(%d, %d)\n", startSelect.row, startSelect.column, endSelect.row, endSelect.column);
            double w = textWidth[lens] - textWidth[startSelect.column - 1];
            double sx = max(0, x - dx + textWidth[startSelect.column - 1]);
            double ex = min(winWidth, x - dx + textWidth[startSelect.column - 1] + w);
            if (w != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        } else if (row == endSelect.row && startSelect.row != endSelect.row) {      //������ѡ������һ��
            double w = textWidth[endSelect.column - 1];
            double sx = max(0, x - dx);
            double ex = min(winWidth, x - dx + w);
            if (w != 0) drawRectangle(sx, y - th * (row - winCurrent.row) - GetFontDescent(), ex - sx, th, 1);
        } else if (row == startSelect.row && startSelect.row == endSelect.row) {    //ѡ�񲿷���ͬһ��
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
        if (clock() % 1000 < 500 && getTypingState()) {     //500ms�ļ����˸����������״̬ʱ����ʾ���
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
    ���Ʋ��Ҵ��� ����һ�����������ı���Ͳ��Ұ�ť��
    ����������ļ��ı������Ͻ�
    �Ȼ��Ƶײ��ı� �ڻ��Ʋ��Ҵ���ʱ�Ȼ���һ����ɫ���θ��ǵ��µ��ı����ڻ��Ʋ��Ҵ���
    �����滻����ͬ��
*/
static void drawFindArea() {
    
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
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

    //չ�����滻���ڵİ�ť
    double buttonFindToReplaceX = x + fH * 0.2;
    double buttonFindToReplaceY = y + fH * 0.2;
    double buttonFindToReplaceW = fH * 1.2;
    double buttonFindToReplaceH = fH * 1.2;

    //�����ı���
    double textboxFindX = x + fH * 1.6;
    double textboxFindY = buttonFindToReplaceY;
    double textboxFindW = w / 2;
    double textboxFindH = buttonFindToReplaceH;

    //�رղ��ҿ�ť
    double buttonCloseX = x + w - fH * 1.4;
    double buttonCloseY = textboxFindY;
    double buttonCloseW = fH * 1.2;
    double buttonCloseH = textboxFindH;

    //������һ����ť
    double buttonFindLastX = buttonCloseX - fH * 1.7;
    double buttonFindLastY = textboxFindY;
    double buttonFindLastW = textboxFindH;
    double buttonFindLastH = textboxFindH;

    //������һ����ť
    double buttonFindNextX = buttonFindLastX - fH * 1.7;
    double buttonFindNextY = textboxFindY;
    double buttonFindNextW = textboxFindH;
    double buttonFindNextH = textboxFindH;

    //����������ݵ��ı���
    SetPenColor("White");
    drawRectangle(textboxFindX, textboxFindY, textboxFindW, textboxFindH, 1);
    SetPenColor(originColor);
    SetPointSize(13);
    if (textbox(GenUIID(0), textboxFindX, textboxFindY, textboxFindW, textboxFindH, inputFindText, sizeof(inputFindText) / sizeof(inputFindText[0]))) {
        
    }
    
    //������һ����ť
    SetPointSize(16);
    if (button(GenUIID(0), buttonFindNextX, buttonFindNextY, buttonFindNextW, buttonFindNextH, "��")) {
        findResult = findNextText(inputFindText);
    }

    //������һ����ť
    if (button(GenUIID(0), buttonFindLastX, buttonFindLastY, buttonFindLastW, buttonFindLastH, "��")) {
        findResult = findLastText(inputFindText);
    }

    //��������޽������ʾ�޽��
    if (!findResult) {
        SetPointSize(13);
        MovePen(textboxFindX + textboxFindW + fH * 0.5, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        SetPenColor("Red");
        DrawTextString("�޽��");
        SetPenColor(originColor);
    }
    
    //������ҵ��ı�����û�����ݣ�����ʾ��ɫ���� ���� ��ʾ
    if (strlen(inputFindText) < 1) {
        SetPenColor("Dark Gray");
        SetPointSize(13);
        MovePen(textboxFindX + GetFontAscent() / 2, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        DrawTextString("����");
        SetPenColor(originColor);
    }
    
    //չ�����滻����
    SetPointSize(18);
    if (button(GenUIID(0), buttonFindToReplaceX, buttonFindToReplaceY, buttonFindToReplaceW, buttonFindToReplaceH, "��")) {
        isShowFind = 0;
        isShowReplace = 1;
        SetFont(originFont);
        display();
        return;
    }

    //�رհ�ť
    if (button(GenUIID(0), buttonCloseX, buttonCloseY, buttonCloseW, buttonCloseH, "��")) {
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
    �����滻���� ���������ı��򣨷ֱ��ǲ��Һ��滻���Լ��滻��ť��
*/
static void drawReplaceArea() {

    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
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

    //�滻�����ı���
    double textboxReplaceX = x + fH * 1.6;
    double textboxReplaceY = y + fH * 0.2;
    double textboxReplaceW = w / 2;
    double textboxReplaceH = fH * 1.2;

    //��ʾ���Ҵ��ڰ�ť
    double buttonReplaceToFindX = x + fH * 0.2;
    double buttonReplaceToFindY = y + fH * 0.2;
    double buttonReplaceToFindW = fH * 1.2;
    double buttonReplaceToFindH = fH * 2.6;

    //���������ı���
    double textboxFindX = x + fH * 1.6;
    double textboxFindY = y + fH * 0.2 + fH * 1.4;
    double textboxFindW = w / 2;
    double textboxFindH = fH * 1.2;

    //�رհ�ť
    double buttonCloseX = x + w - fH * 1.4;
    double buttonCloseY = textboxFindY;
    double buttonCloseW = fH * 1.2;
    double buttonCloseH = textboxFindH;

    //������һ����ť
    double buttonFindLastX = buttonCloseX - fH * 1.7;
    double buttonFindLastY = textboxFindY;
    double buttonFindLastW = textboxFindH;
    double buttonFindLastH = textboxFindH;

    //������һ����ť
    double buttonFindNextX = buttonFindLastX - fH * 1.7;
    double buttonFindNextY = textboxFindY;
    double buttonFindNextW = textboxFindH;
    double buttonFindNextH = textboxFindH;

    //�滻��ť
    const char *replaceButton = "�滻";
    double buttonReplaceX = textboxReplaceX + textboxReplaceW + fH * 0.5;
    double buttonReplaceY = textboxReplaceY;
    double buttonReplaceW = TextStringWidth(replaceButton) * 1.3;
    double buttonReplaceH = textboxReplaceH;

    //�滻ȫ����ť
    const char * replaceAllButton = "�滻ȫ��";
    double buttonReplaceAllX = buttonReplaceX + buttonReplaceW + fH;
    double buttonReplaceAllY = textboxReplaceY;
    double buttonReplaceAllW = TextStringWidth(replaceAllButton) * 1.2;
    double buttonReplaceAllH = textboxReplaceH;

    //ֻ��ʾ���Ҵ���
    SetPointSize(18);
    if (button(GenUIID(0), buttonReplaceToFindX, buttonReplaceToFindY, buttonReplaceToFindW, buttonReplaceToFindH, "��")) {
        isShowFind = 1;
        isShowReplace = 0;
        SetFont(originFont);
        display();
        return;
    }

    //�ر��滻����
    if (button(GenUIID(0), buttonCloseX, buttonCloseY, buttonCloseW, buttonCloseH, "��")) {
        isShowReplace = 0;
        isShowFind = 0;
        setTypingState(1);
        SetFont(originFont);
        SetPointSize(originPointSize);
        display();
        return;
    }

    //����������ݵ��ı���
    SetPenColor("White");
    drawRectangle(textboxFindX, textboxFindY, textboxFindW, textboxFindH, 1);
    SetPenColor(originColor);
    SetPointSize(13);
    if (textbox(GenUIID(0), textboxFindX, textboxFindY, textboxFindW, textboxFindH, inputFindText, sizeof(inputFindText) / sizeof(inputFindText[0]))) {
        
    }
    
    //������һ����ť
    SetPointSize(16);
    if (button(GenUIID(0), buttonFindNextX, buttonFindNextY, buttonFindNextW, buttonFindNextH, "��")) {
        findResult = findNextText(inputFindText);
    }

    //������һ����ť
    if (button(GenUIID(0), buttonFindLastX, buttonFindLastY, buttonFindLastW, buttonFindLastH, "��")) {
        findResult = findLastText(inputFindText);
    }
    
    //������ҵ��ı�����û�����ݣ�����ʾ��ɫ���� ���� ��ʾ
    if (strlen(inputFindText) < 1) {
        SetPenColor("Dark Gray");
        SetPointSize(13);
        MovePen(textboxFindX + GetFontAscent() / 2, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        DrawTextString("����");
        SetPenColor(originColor);
    }

    //��ʾ�滻�ı������ı���
    SetPointSize(13);
    SetPenColor("White");
    drawRectangle(textboxReplaceX, textboxReplaceY, textboxReplaceW, textboxReplaceH, 1);
    SetPenColor(originColor);
    if (textbox(GenUIID(0), textboxReplaceX, textboxReplaceY, textboxReplaceW, textboxReplaceH, inputReplaceText, sizeof(inputReplaceText) / sizeof(inputReplaceText[0]))) {
        
    }
    
    if (strlen(inputReplaceText) < 1) {
        SetPenColor("Dark Gray");
        MovePen(textboxReplaceX + GetFontAscent() / 2, textboxReplaceY + textboxReplaceH / 2 - GetFontAscent() / 2);
        DrawTextString("�滻");
        SetPenColor(originColor);
    }

    //��������޽������ʾ�޽��
    if (!findResult) {
        SetPointSize(13);
        MovePen(textboxFindX + textboxFindW + fH * 0.5, textboxFindY + textboxFindH / 2 - GetFontAscent() / 2);
        SetPenColor("Red");
        DrawTextString("�޽��");
        SetPenColor(originColor);
    }

    //��ʾ�滻��ť
    if (button(GenUIID(0), buttonReplaceX, buttonReplaceY, buttonReplaceW, buttonReplaceH, replaceButton)) {
        replaceText(inputFindText, inputReplaceText);
    }

    //��ʾ�滻ȫ����ť
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
static int initSetting = 1;     //1: ��Ҫ��ʼ��
static TextStyle tmpStyle;

static void drawSettingPage() {
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
    DrawTextString("��������ϵ�С�����ʹ�õȿ����塣");
    SetPenColor(originColor);
    if (textbox(GenUIID(0), x + w, y - fD * 2 - fH * 1.1, tw, th, fontFamily, sizeof(fontFamily) / sizeof(fontFamily[0]))) {

    }
    tmpStyle.fontFamily = CopyString(fontFamily);

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
        tmpStyle.fontSize = tmpFontSize;
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
        tmpStyle.lineSpacing = tmpLineSpacing;
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
            tmpStyle.textColor = CopyString(textColor);
        } else {
            SetPenColor("Red");
            MovePen(x + w + tw + fH, y - h * 3 - fH - fD);
            DrawTextString("��Ч��ʮ��������ɫ��");
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
            tmpStyle.backgroundColor = CopyString(backgroundColor);
        } else {
            SetPenColor("Red");
            MovePen(x + w + tw + fH, y - h * 4 - fH - fD);
            DrawTextString("��Ч��ʮ��������ɫ��");
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
    SetFont(tmpStyle.fontFamily);
    SetPointSize(tmpStyle.fontSize);
    fH = GetFontHeight();
    h = fH * tmpStyle.lineSpacing;
    H = max(H, h * 5);
    char *text[] = {
        "����һ��ʵ���ı�",
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
 * ���Ƽ��̿�ݼ�ҳ��
 */ 
static void drawKeyboardPage() {
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
    SetPointSize(19);
    fH = GetFontHeight();
    double fD = GetFontDescent();
    h = GetFontHeight() * 1.2;

    static char *tableContent[][2] = {
        "����", "����",
        "���ļ�", "Ctrl + O",
        "�����ļ�", "Ctrl + S",
        "���Ϊ", "Ctrl + Shift + S",
        "�˳�Notepad--", "Ctrl + W",
        "�༭������", "Ctrl + Z",
        "�༭������", "Ctrl + Y / Ctrl + Shift + Z",
        "�༭������", "Ctrl + X",
        "�༭������", "Ctrl + C",
        "�༭��ճ��", "Ctrl + V",
        "�༭������", "Ctrl + F",
        "�༭���滻", "Ctrl + H",
        "�༭��ȫѡ", "Ctrl + A",
        "��/���沢�˳�����", "Ctrl + E",
        "��/�رռ��̿�ݼ�����", "Ctrl + K",
        "����", "F1"
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
 * ���ƹ���ҳ��
 */
static void drawAboutPage() {
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

    SetPointSize(20);
    fH = GetFontHeight();
    double fD = GetFontDescent();
    h = GetFontHeight() * 1.5;
    x = winWidth * 3 / 32;
    y = winHeight * 6 / 7 - h * 2;

    MovePen(x, y);
    DrawTextString("Notepad-- ������ɽ��ղ�����ܽ���ͬ������");
    MovePen(x, y - h);
    DrawTextString("��Ӧ��ʹ�õ�����ͼ�ο� libgraphics �Լ��㽭��ѧC�̿γ��鿪���� simpleGUI ͼ�ο⡣");
    MovePen(x, y - h * 2);
    DrawTextString("���Ƕ����õĵ�����ͼ�ο������Щ���޸ģ�����Ҳ�õ��� Windows GDI��");
    MovePen(x, y - h * 3);
    DrawTextString("��Ӧ������ Github �Ͽ�Դ��");
    setButtonColors("White", "Link Blue", "Button Gray", "Link Hot Blue", 1);
    if (button(GenUIID(0), x, y - h * 4 - fD, TextStringWidth("�˽����") * 1.3, fH * 1.2, "�˽����")) {
        WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
    }

    SetPenColor(originColor);
    SetFont(originFont);
    SetPointSize(originPointSize);
}

/**
 * �����Ҽ��˵�
 */ 
static void drawContextMenu() {
    
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
    SetPointSize(13);

    static char *contextMenuList[] = {
        "����                           Ctrl+Z",
        "����                           Ctrl+Y",
        "����                           Ctrl+X",
        "����                           Ctrl+C",
        "ճ��                           Ctrl+V",
        "ȫѡ                           Ctrl+A"
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
            case 0:     //����
                undo();
                break;

            case 1:     //����
                redo();
                break;
            
            case 2:     //����
                shearText();
                break;

            case 3:     //����
                copyText();
                break;

            case 4:     //ճ��
                pasteText();
                break;
            
            case 5:{    //ȫѡ
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
 * ����겻�ڴ����У����ƶ�����ʹ����ڴ�����
 */ 
void setCursorInWindow() {
    RCNode cursor = getCursorRC();
    RCNode winCurrent = getWindowCurrentRC();

    char *originFont = GetFont();
    int originPointSize = GetPointSize();

    SetFont("΢���ź�");
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
    int totl = ceil((oy + GetFontAscent() - minY) / h);     //��������ʾ��������
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    if (winCurrent.row > cursor.row - 1) {  //����ڴ����Ϸ�
        winCurrent.row = cursor.row;
    } else {    //����ڴ����·�
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
        if (s[i] == '\t') {     //�����Ʊ��
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
    if (ox - dx + tx < ox) {    //����ڴ�����
        winCurrent.column = max(winCurrent.column - ceil((dx - tx) / TextStringWidth(" ")), 1);
    } else {
        if (ox - dx + tx > winWidth - ox) {     //����ڴ����ҷ�
            winCurrent.column = winCurrent.column + ceil((ox - dx + tx - winWidth + ox) / TextStringWidth(" "));
        }
    }

    setWindowCurrentRC(winCurrent);

    SetFont(originFont);
    SetPointSize(originPointSize);
}

/**
 * �����ڵ�(x, y)��������ת��Ϊ��������
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

    SetFont("΢���ź�");
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

    for (int row = winCurrent.row - 1; row < min(winCurrent.row + totl, getTotalRow() + 1); row++) {    //row:��ǰ��ʾ����
        if (oy - th * (row - winCurrent.row) - GetFontDescent() <= ny && ny <= oy + th * 2 - GetFontDescent()) {
            mouse.row = row;
        } else continue;
        string s = getRowContent(row);
        if (s == NULL) continue;
        int i = 0, j = 0, lastj = 0, lens = getRowLength(row);
        double w = 0;
        if (s[lens - 1] == '\n') lens--;
        while (i < lens) {      //i:��s�е��±� j:��t�е��±�λ��
            if (s[i] == '\t') {     //�����Ʊ��
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
                    if (i >= 1 && s[i - 1] & 0x80) mouse.column = i - 1;      //������
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

    if (!mouse.row) {   //���������ĩβ����
        mouse.row = getTotalRow();
        mouse.column = getRowLength(mouse.row) + 1;
    }

    // printf("MOUSE:%d %d\n", mouse.row, mouse.column);

    SetPointSize(originPointSize);
    SetFont(originFont);
    return mouse;
}

/**
 * �����Ҽ��˵������Ͻ�λ��
 */ 
void setContextMenuXY(double x, double y) {
    contextMenuX = x;
    contextMenuY = y;
} 


/**
 * ���ڴ�����̰���ʱ�Ŀ�ݼ�
 * key      ���µİ���
 * isShift  �Ƿ���Shift
 * isCtrl   �Ƿ���Ctrl
 * isTyping �Ƿ�������״̬
 */ 
void processShortcutKey(int key, int isShift, int isCtrl, int isTyping) {
    if (isCtrl && getTextDisplayState()) {  //���ı��༭���ڵĿ�ݼ�
        switch (key) {
            case 'N':   //Ctrl + N  �½�
                createFile();
                break;
            case 'O':   //Ctrl + O  ��
                openFile();
                break;
            case 'S':   
                if (isShift) {
                    saveAnother();  //Ctrl + Shift + S  ���Ϊ
                } else {
                    saveFile(); //Ctrl + S  ����
                }
                break;
            case 'W':   //Ctrl + W  �˳�
                exitApplication();
                break;
            case 'Z':   
                if (isShift) {
                    redo(); //Ctrl + Shift + Z  ����
                } else {
                    undo(); //Ctrl + Z  ����
                }
                break;
            case 'Y':   //Ctrl + Y  ����
                redo();
                break;
            case 'X':   //Ctrl + X  ����
                shearText();
                break;
            case 'C':   //Ctrl + C  ����
                copyText();
                break;
            case 'V':   //Ctrl + V  ճ��
                pasteText();
                break;
            case 'F':   //Ctrl + F  ����
                isShowFind ^= 1;
                break;
            case 'H':   //Ctrl + H  �滻
                isShowReplace ^= 1;
                break;
            case 'A':   //Ctrl + A  ȫѡ
            {
                int totr = getTotalRow();
                int totc = getRowLength(totr) + 1;
                setSelectStartRC((RCNode) {1, 1});
                setSelectEndRC((RCNode) {totr, totc});
                setCursorRC((RCNode) {totr, totc});
                break;
            }
            case 'E':   //Ctrl + E  ���ý���
                isShowSetting = 1;
                break;
            case 'K':   //Ctrl + K  ���̿�ݼ�����
                isShowKeyboard = 1;
                break;
        }
    } else if (isCtrl && isShowSetting) { //���ý���
        switch (key) {
            case 'E':   //Ctrl + E  ���ý���
            case 'K':   //Ctrl + K  ���̿�ݼ�����
                isShowSetting = 0;
                initSetting ^= 1;
                setTextStyle(tmpStyle);
                if (key == 'K') isShowKeyboard = 1;
                break;
            case 'W':   //Ctrl + W  �˳�
                exitApplication();
                break;
        }
    } else if (isCtrl && isShowKeyboard) {    //���̿�ݼ�����
        switch (key) {
            case 'E':   //Ctrl + E  ���ý���
            case 'K':   //Ctrl + K  ���̿�ݼ�����
                isShowKeyboard = 0;
                if (key == 'E') isShowSetting = 1;
                break;
            case 'W':   //Ctrl + W  �˳�
                exitApplication();
                break;
        }
    } else if (isCtrl && isShowAbout) {   //���ڽ���
        switch (key) {
            case 'E':   //Ctrl + E  ���ý���
                isShowAbout = 0;
                isShowSetting = 1;
                break;
            case 'K':   //Ctrl + K  ���̿�ݼ�����
                isShowAbout = 0;
                isShowSetting = 1;
                break;
            case 'W':   //Ctrl + W  �˳�
                exitApplication();
                break;
        }
    }
    if (key == VK_F1) {     //F1    ����
        WinExec("cmd.exe /k start https://github.com/PeiPei233/UnableToCount", SW_HIDE);
    }
}

/**
 * ��ʾ��������
 */ 
void display() {
    if (lockDisplay) {  //�����display��û������������
        return;
    }

    lockDisplay = 1;    //����

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

    lockDisplay = 0;    //����

}