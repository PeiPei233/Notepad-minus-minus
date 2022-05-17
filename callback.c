/*
    ��Ҫ������� ���̰����Ļص�
*/

//TODO:�������ҷ����ʱ�����ַ��Ĵ���

#include "global.h"
#include "libgraphics.h"
#include "display.h"
#include "file.h"
#include "init.h"
#include "math.h"
#include "imgui.h"

static int isButtonDown = 0;
static int isShift = 0;
static int isTyping = 1;
static char t[100010];

/**
 * ��ȡ��ǰ����״̬
 */
int getTypingState() {
    return isTyping;
}

/**
 * ���õ�ǰ����״̬
 */ 
void setTypingState(int newTypingState) {
    isTyping = newTypingState;
}

/**
 * �����ڵ�(x, y)��������ת��Ϊ��������
 */ 
RCNode XYtoRC(int x, int y) {
    
    char *s = getCurrentString();
    int lens = strlen(s);
    if (!lens) return (RCNode) {1, 1};

    double nx = ScaleXInches(x);
    double ny = ScaleYInches(y);

    RCNode winCurrent = getWindowCurrentRC();
    TextStyle style = getTextStyle();

    int originPointSize = GetPointSize();
    char *originFont = GetFont();

    SetFont("΢���ź�");
    SetPointSize(13);

    double menuBarH = GetFontHeight() * 1.5;

    SetFont(style.fontFamily);
    SetPointSize(style.fontSize);
    // printf("MOUSESTYLE:%s %d %lf\n", style.fontFamily, style.fontSize, style.lineSpacing);
    double fH = GetFontHeight();
    double ox = menuBarH / 1.5 * 2 / 3;
    double th = fH * style.lineSpacing;
    double oy = winHeight - menuBarH - th * 1.25;

    int i = 0, j = 0, k = 0, curl = 0, totl = 0;
    RCNode mouse = (RCNode) {0, 0};

    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    i = j = 0;
    while (i < lens) {
        totl++;
        t[0] = t[1] = 0;
        if (winCurrent.row == 1) {
            if (oy - th * (totl - 1) - GetFontDescent() <= ny && ny <= oy + th * 2 - GetFontDescent()) {
                mouse.row = totl;
            }
            if (oy - th * (totl - 2) - GetFontDescent() < 0) break;
        } else {
            if (totl >= winCurrent.row - 1) {
                curl++;
                if (oy - th * (curl - 2) - GetFontDescent() <= ny && ny <= oy - th * (curl - 3) - GetFontDescent()) {
                    mouse.row = totl;
                } else {
                    while (i < strlen(s) && s[i] != '\n') {
                        i++;
                    }
                }
                if (oy - th * (curl - 3) - GetFontDescent() < 0) break;
            }
        }
        while (i < strlen(s) && s[i] != '\n') {
            if (s[i] == '\t') {
                i++;
                k++;
                if (j % 4 == 0) {
                    for (int _i = 1; _i <= 4; _i++) {
                        t[j] = ' ';
                        j++;
                        t[j] = t[j + 1] = 0;
                    }
                } else while (j % 4) {
                    t[j] = ' ';
                    j++;
                    t[j] = t[j + 1] = 0;
                }
            } else {
                if (s[i] & 0x80 && s[i + 1] & 0x80) {
                    t[j] = s[i];
                    i++;
                    j++;
                    k++;
                    t[j] = t[j + 1] = 0;
                }
                t[j] = s[i];
                i++;
                j++;
                t[j] = t[j + 1] = 0;
                k++;
            }
            if (mouse.row) {
                if (nx <= ox - dx + TextStringWidth(t)) {
                    i = lens + 1;
                    mouse.column = k;
                    if (k >= 2 && t[k - 1] & 0x80 && t[k - 2] & 0x80) mouse.column = k - 1;
                    break;
                }
            }
        }
        if (mouse.row) {
            if (!mouse.column) mouse.column = k + 1;
            break;
        }
        j = k = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }
    if (!mouse.row) {
        mouse.row = totl;
        mouse.column = getRowLen(totl);
    }
    // printf("MOUSE:%d %d\n", mouse.row, mouse.column);

    SetPointSize(originPointSize);
    SetFont(originFont);
    return mouse;
}

/*
    �Զ��崫��������統ǰ���꣬���ֻ��Ʒ�Χ���иߵȣ� ʵ�ָ��ݵ�ǰ��(x, y)���귵�����ļ��е���������(r, c)
    r: row ��
    c: column ��
    ������ʱ���ĵ�ǰ��ʾ���ı���Χ���Ͻ�����λ��
    ������²��϶�ʱ����ѡ��
    ѡ�����λ��Ӧ����ѡ��Χ�����
*/
void getMouse(int x, int y, int button, int event) {

    double nx = ScaleXInches(x);
    double ny = ScaleYInches(y);

    UIState gs_UIState = getUIState();
    // printf("CLICKITEM:%d\n", gs_UIState.clickedItem);
    if (gs_UIState.clickedItem) {
        isTyping = 0;
        return;
    } else {
        isTyping = 1;
    }

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
    SetPointSize(13);

    double fH = GetFontHeight();
    double w = TextStringWidth("�ļ�") * 2;

    SetFont(originFont);
    SetPointSize(originPointSize);

    if (ny >= winHeight - fH * 1.5) return;
    if (ny <= fH * 1.4) return;

    double minY = fH * 1.4;
    double menuBarH = fH * 1.5;

    if (getFindDisplayState() && event == BUTTON_DOWN && button == LEFT_BUTTON) {
        if (nx > winWidth / 2 && nx < winWidth / 2 + winWidth / 2 * 15 / 16 &&
            ny > winHeight - fH * 3.5 && ny < winHeight - fH * 3.5 + fH * 1.6) {
            isTyping = 0;
        }
    }

    if (getReplaceDisplayState() && event == BUTTON_DOWN && button == LEFT_BUTTON) {
        if (nx > winWidth / 2 && nx < winWidth / 2 + winWidth / 2 * 15 / 16 &&
            ny > winHeight - fH * 4.9 && ny < winHeight - fH * 4.9 + fH * 3) {
            isTyping = 0;
        }
    }

    if (!getTextDisplayState()) {
        isTyping = 0;
    } else {
        if (event == BUTTON_DOWN && button == LEFT_BUTTON) {
            if (getFindDisplayState()) {
                if (!(nx > winWidth / 2 && nx < winWidth / 2 + winWidth / 2 * 15 / 16 &&
                    ny > winHeight - fH * 3.5 && ny < winHeight - fH * 3.5 + fH * 1.6)) {
                    isTyping = 1;
                } else isTyping = 0;
            } else if (getReplaceDisplayState()) {
                if (!(nx > winWidth / 2 && nx < winWidth / 2 + winWidth / 2 * 15 / 16 &&
                    ny > winHeight - fH * 4.9 && ny < winHeight - fH * 4.9 + fH * 3)) {
                    isTyping = 1;
                } else isTyping = 0;
            } else {
                isTyping = 1;
            }
        }
        
    }

    // printf("ISTYPYING:%d\n", isTyping);

    if (getFindDisplayState()) {
        if (nx > winWidth / 2 && nx < winWidth / 2 + winWidth / 2 * 15 / 16 &&
            ny > winHeight - fH * 3.5 && ny < winHeight - fH * 3.5 + fH * 1.6) {
            return;
        }
    }

    if (getReplaceDisplayState()) {
        if (nx > winWidth / 2 && nx < winWidth / 2 + winWidth / 2 * 15 / 16 &&
            ny > winHeight - fH * 4.9 && ny < winHeight - fH * 4.9 + fH * 3) {
            return;
        }
    }

    if (!getTextDisplayState()) {
        return;
    }

    TextStyle style = getTextStyle();
    SetFont(style.fontFamily);
    SetPointSize(style.fontSize);
    fH = GetFontHeight();
    double h = fH * style.lineSpacing;
    double ox = menuBarH / 1.5 * 2 / 3;
    double oy = winHeight - menuBarH - h * 1.25;
    int totl = ceil((oy + GetFontAscent() - minY) / h);

    switch (event) {
        case BUTTON_DOWN:
            if (button == LEFT_BUTTON) {
                RCNode mouse = XYtoRC(x, y);
                isButtonDown = 1;
                setCursorRC(mouse);
                setSelectStartRC(mouse);
                setSelectEndRC(mouse);
            }
            break;
        case MOUSEMOVE:
            if (isButtonDown) {
                RCNode mouse = XYtoRC(x, y);
                setCursorRC(mouse);
                setSelectEndRC(mouse);
                RCNode winCurrent = getWindowCurrentRC();
                if (mouse.row < winCurrent.row) {
                    winCurrent.row = mouse.row;
                    setWindowCurrentRC(winCurrent);
                }
                if (ny <= oy + GetFontAscent() - (totl - 1) * h) {
                    winCurrent.row++;
                    setWindowCurrentRC(winCurrent);
                }
                if (nx <= fH) {
                    winCurrent.column = max(winCurrent.column - 1, 1);
                    setWindowCurrentRC(winCurrent);
                }
                if (nx >= winWidth - fH) {
                    winCurrent.column++;
                    setWindowCurrentRC(winCurrent);
                }
            }
            break;
        case BUTTON_UP:
            if (button == LEFT_BUTTON) {
                RCNode mouse = XYtoRC(x, y);
                isButtonDown = 0;
                RCNode winCurrent = getWindowCurrentRC();
                if (mouse.row < winCurrent.row) {
                    winCurrent.row = mouse.row;
                    setWindowCurrentRC(winCurrent);
                }
                if (ny <= oy + GetFontAscent() - (totl - 1) * h) {
                    winCurrent.row++;
                    setWindowCurrentRC(winCurrent);
                }
                if (nx <= fH) {
                    winCurrent.column = max(winCurrent.column - 1, 1);
                    setWindowCurrentRC(winCurrent);
                }
                if (nx >= winWidth - fH) {
                    winCurrent.column++;
                    setWindowCurrentRC(winCurrent);
                }
                setCursorRC(mouse);
                setSelectEndRC(mouse);
            }
            break;
        case ROLL_UP: 
            if (!isShift) {
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {max(winCurrent.row - (ceil(winWidth / 12 / h)), 1), winCurrent.column});
            } else {    //���󻬶�
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {winCurrent.row, max(winCurrent.column - (ceil(winHeight / 12 / TextStringWidth(" "))), 1)});
            }
            break;
        case ROLL_DOWN:
            if (!isShift) {
                RCNode winCurrent = getWindowCurrentRC();
                int totr = getTotalRow();
                setWindowCurrentRC((RCNode) {min(winCurrent.row + (ceil(winWidth / 12 / h)), totr), winCurrent.column});
            } else {    //���һ���
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {winCurrent.row, winCurrent.column + (ceil(winHeight / 12 / TextStringWidth(" ")))});
            }
            break;
        case BUTTON_DOUBLECLICK:
            break;

    }

    
    SetFont(originFont);
    SetPointSize(originPointSize);
}

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
    int totl = ceil((oy + GetFontAscent() - minY) / h);     //��������ʾ���ܺ���
    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    if (winCurrent.row > cursor.row - 1) {  //����ڴ����Ϸ�
        winCurrent.row = cursor.row;
    } else {    //����ڴ����·�
        if (winCurrent.row + totl - 2 < cursor.row + 1) {
            winCurrent.row = cursor.row - totl + 2;
        }
    }
    
    string s = getCurrentString();
    int i = numofFormerWords((RCNode) {cursor.row, 1});
    char ch = s[i + cursor.column - 2];
    s[i + cursor.column - 2] = 0;
    double tx = TextStringWidth(s + i);
    s[i + cursor.column - 2] = ch;
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

/*
    ���ݴ�����ַ������еȣ�ʵ�ֽ������������ӣ����룩����ǰ�ļ���ĳ��ĳ��(r, c)�Ĺ��ܡ�
*/
void inputChar(char ch) {
    // printf("INPUT:%d\n", ch);
    if (!isTyping) return;
    if (ch == 23) {
        ExitGraphics();
        return;
    }
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    if (!(startSelect.row == endSelect.row && startSelect.column == endSelect.column)) {
        deleteSelectString();
        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
            RCNode t = startSelect;
            startSelect = endSelect;
            endSelect = t;
        }
        setSelectStartRC(startSelect);
        setSelectEndRC(startSelect);
        setCursorRC(startSelect);
    }
    RCNode cursor = getCursorRC();
    if (ch == '\r') {   //�س��ļ��̻ص��ַ�Ϊ'\r'
        ch = '\n';
        addChar(ch);
        cursor.row++;
        cursor.column = 1;
        setCursorRC(cursor);
        setSelectStartRC(cursor);
        setSelectEndRC(cursor);
        return;
    } else if (ch > 0 && ch < 32 && ch != '\t') {
        return;
    }
    addChar(ch);
    cursor.column++;
    setCursorRC(cursor);
    setCursorInWindow();
}

/*
    ���ݴ���ļ������룬�ж��Ƿ���Ҫ���С��˸�Ȳ���
    �������Ҽ�ʱ�ƶ����λ�ã����ı���������
    Shift+�����ʱ�ı�ѡ��Χ�����ѹ��ŵ�ѡ��Χ��ͷ
*/
void inputKeyboard(int key, int event) {

    if (event == KEY_DOWN) {
        switch (key) {
            //������
            case VK_LEFT: {
                RCNode currentCursor = getCursorRC();
                if (currentCursor.column == 1) {
                    if (currentCursor.row > 1) {
                        int len = getRowLen(currentCursor.row - 1);
                        setCursorRC((RCNode) {currentCursor.row - 1, len});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row - 1, len});
                        setSelectEndRC((RCNode) {currentCursor.row - 1, len});
                    }
                } else {
                    string s = getCurrentString();
                    int i = numofFormerWords(currentCursor);
                    if (s[i - 1] & 0x80) {  //������
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column - 2});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, currentCursor.column - 2});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column - 2});
                    } else {
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                    }
                }
                setCursorInWindow();
                break;
            }
            //������
            case VK_RIGHT: {
                RCNode currentCursor = getCursorRC();
                if (currentCursor.column == getRowLen(currentCursor.row)) {
                    if (currentCursor.row < getTotalRow()) {
                        setCursorRC((RCNode) {currentCursor.row + 1, 1});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row + 1, 1});
                        setSelectEndRC((RCNode) {currentCursor.row + 1, 1});
                    }
                } else {
                    string s = getCurrentString();
                    int i = numofFormerWords(currentCursor);
                    if (s[i] & 0x80) {  //������
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column + 2});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, currentCursor.column + 2});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column + 2});
                    } else {
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                    }
                }
                setCursorInWindow();
                break;
            }
            //SHIFT
            case VK_SHIFT:
                isShift = 1;
                break;
            //������
            case VK_UP: {
                RCNode currentCursor = getCursorRC();
                if (currentCursor.row == 1) {   //��ǰ����ڵ�һ��
                    setCursorRC((RCNode) {1, 1});
                    if (!isShift) setSelectStartRC((RCNode) {1, 1});
                    setSelectEndRC((RCNode) {1, 1});
                } else {
                    int begin = numofFormerWords((RCNode) {currentCursor.row - 1, 1});
                    string s = getCurrentString();
                    int i = begin, lens = strlen(s);
                    while (i < lens && s[i] != '\n') {
                        if (currentCursor.column == i - begin + 1 || (currentCursor.column == i - begin + 2 && s[i] & 0x80)) {
                            currentCursor.row--;
                            currentCursor.column = i - begin + 1;
                            setCursorRC(currentCursor);
                            if (!isShift) setSelectStartRC(currentCursor);
                            setSelectEndRC(currentCursor);
                            break;
                        }
                        if (s[i] & 0x80) i++;   //��������
                        i++;
                    }
                    if (s[i] == '\n' || s[i] == lens) {
                        int len = i - begin + 1;
                        setCursorRC((RCNode) {currentCursor.row - 1, len});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row - 1, len});
                        setSelectEndRC((RCNode) {currentCursor.row - 1, len});
                    }
                }
                setCursorInWindow();
                break;
            }
            //������
            case VK_DOWN: {
                RCNode currentCursor = getCursorRC();
                if (currentCursor.row == getTotalRow()) {   //�Ƿ������һ��
                    int len = getRowLen(currentCursor.row);
                    setCursorRC((RCNode) {currentCursor.row, len});
                    if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, len});
                    setSelectEndRC((RCNode) {currentCursor.row, len});
                } else {
                    int begin = numofFormerWords((RCNode) {currentCursor.row + 1, 1});
                    string s = getCurrentString();
                    int i = begin, lens = strlen(s);
                    while (i < lens && s[i] != '\n') {
                        if (currentCursor.column == i - begin + 1 || (currentCursor.column == i - begin + 2 && s[i] & 0x80)) {
                            currentCursor.row++;
                            currentCursor.column = i - begin + 1;
                            setCursorRC(currentCursor);
                            if (!isShift) setSelectStartRC(currentCursor);
                            setSelectEndRC(currentCursor);
                            break;
                        }
                        if (s[i] & 0x80) i++;   //��������
                        i++;
                    }
                    if (s[i] == '\n' || i == lens) {
                        int len = i - begin + 1;
                        setCursorRC((RCNode) {currentCursor.row + 1, len});
                        if (!isShift) setSelectStartRC((RCNode) {currentCursor.row + 1, len});
                        setSelectEndRC((RCNode) {currentCursor.row + 1, len});
                    }
                }
                setCursorInWindow();
                break;
            }
            //END
            case VK_END: {
                RCNode currentCursor = getCursorRC();
                int len = getRowLen(currentCursor.row);
                setCursorRC((RCNode) {currentCursor.row, len});
                if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, len});
                setSelectEndRC((RCNode) {currentCursor.row, len});

                setCursorInWindow();
                break;
            }
            //HOME
            case VK_HOME: {
                RCNode currentCursor = getCursorRC();
                setCursorRC((RCNode) {currentCursor.row, 1});
                if (!isShift) setSelectStartRC((RCNode) {currentCursor.row, 1});
                setSelectEndRC((RCNode) {currentCursor.row, 1});
                
                setCursorInWindow();
                break;
            }
            //BACKSPACE
            case VK_BACK: 
                if (isTyping) {
                    RCNode cursor = getCursorRC();
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    if (!(startSelect.column == endSelect.column && startSelect.row == endSelect.row)) {
                        deleteSelectString();
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setCursorRC(startSelect);
                        setSelectEndRC(startSelect);
                        return;
                    }
                    int i = numofFormerWords(cursor);
                    string s = getCurrentString();
                    if (i == 0) break;
                    if (s[i - 1] == '\n') {
                        int t = getRowLen(cursor.row - 1);
                        deleteChar();
                        cursor.row--;
                        cursor.column = t;
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    } else {
                        if (s[i - 1] & 0x80) {  //��������
                            deleteChar();
                            cursor.column--;
                            setCursorRC(cursor);
                            setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                        }
                        deleteChar();
                        cursor.column--;
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    }
                    setCursorInWindow();
                }
                break;
            //DELETE
            case VK_DELETE: 
                if (isTyping) {
                    RCNode cursor = getCursorRC();
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    if (!(startSelect.column == endSelect.column && startSelect.row == endSelect.row)) {
                        deleteSelectString();
                        setCursorRC(startSelect);
                        setSelectEndRC(startSelect);
                        return;
                    }
                    int i = numofFormerWords(cursor);
                    string s = getCurrentString();
                    if (i == strlen(s) - 1) return;
                    if (s[i] == '\n') {
                        setCursorRC((RCNode) {cursor.row + 1, 1});
                        deleteChar();
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    } else {
                        if (s[i] & 0x80) {  //��������
                            setCursorRC((RCNode) {cursor.row, cursor.column + 2});
                            deleteChar();
                            setCursorRC(cursor);
                            setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                        }
                        setCursorRC((RCNode) {cursor.row, cursor.column + 1});
                        deleteChar();
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    }
                    setCursorInWindow();
                }
                break;
            //TAB
            // case VK_TAB:
            //     puts("TAB");
            //     break;
            //ENTER
            // case VK_RETURN:
            //     puts("ENTER");
            //     break;
            case VK_ESCAPE:
                if (getFindDisplayState()) setFindDisplayState(0);
                if (getReplaceDisplayState()) setReplaceDisplayState(0);
                break;
        }
    }
    if (event == KEY_UP) {
        switch (key) {
            case VK_SHIFT:
                isShift = 0;
                break;
        }
    }
    
    display();
}