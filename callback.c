/*
    ��Ҫ������� ���̰����Ļص�
*/

#include "global.h"
#include "libgraphics.h"
#include "display.h"
#include "file.h"

int isButtonDown = 0;
int isShift = 0;
static char t[100010];

/**
 * ��õ� row �е�������
 */ 
int getRowLen(int row) {
    char *s = getCurrentString();
    RCNode winCurrent = getWindowCurrentRC();

    int i = 0, j = 0, cntl = 0, lens = strlen(s);
    
    t[0] = t[1] = 0;
    while (i < lens) {
        cntl++;
        t[0] = t[1] = 0;
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        if (cntl == row) {
            return j + 1;
        } 
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }
    return 0;
}

/**
 * ����ļ�������
 */ 
int getTotalRow() {
    char *s = getCurrentString();

    int i = 0, j = 0, cntl = 0, lens = strlen(s);
    t[0] = t[1] = 0;
    while (i < lens) {
        cntl++;
        t[0] = t[1] = 0;
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
        }
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }
    return cntl;
}

/**
 * �����ڵ�(x, y)��������ת��Ϊ��������
 */ 
RCNode XYtoRC(int x, int y) {
    
    double nx = ScaleXInches(x);
    double ny = ScaleYInches(y);

    char *s = getCurrentString();
    RCNode winCurrent = getWindowCurrentRC();

    int originPointSize = GetPointSize();
    SetPointSize(15);
    double fH = GetFontHeight();
    double ox = fH;
    double oy = winHeight - fH * 3;
    double w = winWidth - fH * 2;
    double th = fH * 1.2;

    int i = 0, j = 0, k = 0, cntl = 0, lens = strlen(s);
    RCNode mouse = (RCNode) {0, 0};

    double dx = TextStringWidth(" ") * (winCurrent.column - 1);

    i = j = 0;
    while (i < lens) {
        cntl++;
        t[0] = t[1] = 0;
        if (winCurrent.row == 1) {
            if (oy - th * (cntl - 1) - GetFontDescent() <= ny && ny <= oy + th - GetFontDescent()) {
                mouse.row = cntl;
            }
        } else {
            if (cntl >= winCurrent.row - 1) {
                k++;
                if (oy - th * (k - 2) - GetFontDescent() <= ny && ny <= oy - th * (k - 3) - GetFontDescent()) {
                    mouse.row = cntl;
                }
            }
        }
        while (i < strlen(s) && s[i] != '\n') {
            t[j] = s[i];
            i++;
            j++;
            t[j] = t[j + 1] = 0;
            if (mouse.row) {
                if (nx <= ox - dx + TextStringWidth(t)) {
                    i = lens + 1;
                    mouse.column = j;
                    break;
                }
            }
        }
        if (mouse.row) {
            if (!mouse.column) mouse.column = j + 1;
            break;
        }
        j = 0;
        t[0] = t[1] = 0;
        if (s[i] == '\n') i++;
    }
    if (!mouse.row) {
        mouse.row = cntl;
        mouse.column = getRowLen(cntl);
    }
    // printf("MOUSE:%d %d\n", mouse.row, mouse.column);

    SetPointSize(originPointSize);
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
            }
            break;
        case BUTTON_UP:
            if (button == LEFT_BUTTON) {
                RCNode mouse = XYtoRC(x, y);
                isButtonDown = 0;
                setCursorRC(mouse);
                setSelectEndRC(mouse);
            }
            break;
        case ROLL_UP: 
            if (!isShift) {
                RCNode winCurrent = getWindowCurrentRC();
                if (winCurrent.row != 1) {
                    setWindowCurrentRC((RCNode) {winCurrent.row - 1, winCurrent.column});
                }
            } else {
                RCNode winCurrent = getWindowCurrentRC();
                if (winCurrent.column != 1) {
                    setWindowCurrentRC((RCNode) {winCurrent.row, winCurrent.column - 1});
                }
            }
            break;
        case ROLL_DOWN:
            if (!isShift) {
                RCNode winCurrent = getWindowCurrentRC();
                if (winCurrent.row != getTotalRow()) {
                    setWindowCurrentRC((RCNode) {winCurrent.row + 1, winCurrent.column});
                }
            } else {
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {winCurrent.row, winCurrent.column + 1});
            }
            break;

    }
}

/*
    ���ݴ�����ַ������еȣ�ʵ�ֽ������������ӣ����룩����ǰ�ļ���ĳ��ĳ��(r, c)�Ĺ��ܡ�
*/
void inputChar(char ch) {
    printf("INPUT:%d\n", ch);
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
            case VK_LEFT:
                if (!isShift) {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.column == 1) {
                        if (currentCursor.row > 1) {
                            int len = getRowLen(currentCursor.row - 1);
                            setCursorRC((RCNode) {currentCursor.row - 1, len});
                            setSelectStartRC((RCNode) {currentCursor.row - 1, len});
                            setSelectEndRC((RCNode) {currentCursor.row - 1, len});
                        }
                    } else {
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                        setSelectStartRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                    }
                } else {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.column == 1) {
                        if (currentCursor.row > 1) {
                            int len = getRowLen(currentCursor.row - 1);
                            setCursorRC((RCNode) {currentCursor.row - 1, len});
                            setSelectEndRC((RCNode) {currentCursor.row - 1, len});
                        }
                    } else {
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column - 1});
                    }
                }
                break;
            //������
            case VK_RIGHT:
                if (!isShift) {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.column == getRowLen(currentCursor.row)) {
                        if (currentCursor.row < getTotalRow()) {
                            setCursorRC((RCNode) {currentCursor.row + 1, 1});
                            setSelectStartRC((RCNode) {currentCursor.row + 1, 1});
                            setSelectEndRC((RCNode) {currentCursor.row + 1, 1});
                        }
                    } else {
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                        setSelectStartRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                    }
                } else {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.column == getRowLen(currentCursor.row)) {
                        if (currentCursor.row < getTotalRow()) {
                            setCursorRC((RCNode) {currentCursor.row + 1, 1});
                            setSelectEndRC((RCNode) {currentCursor.row + 1, 1});
                        }
                    } else {
                        setCursorRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                        setSelectEndRC((RCNode) {currentCursor.row, currentCursor.column + 1});
                    }
                }
                break;
            //SHIFT
            case VK_SHIFT:
                isShift = 1;
                break;
            //������
            case VK_UP:
                if (!isShift) {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.row == 1) {
                        setCursorRC((RCNode) {1, 1});
                        setSelectStartRC((RCNode) {1, 1});
                        setSelectEndRC((RCNode) {1, 1});
                    } else {
                        int len = getRowLen(currentCursor.row - 1);
                        if (currentCursor.column <= len) {
                            setCursorRC((RCNode) {currentCursor.row - 1, currentCursor.column});
                            setSelectStartRC((RCNode) {currentCursor.row - 1, currentCursor.column});
                            setSelectEndRC((RCNode) {currentCursor.row - 1, currentCursor.column});
                        } else {
                            setCursorRC((RCNode) {currentCursor.row - 1, len});
                            setSelectStartRC((RCNode) {currentCursor.row - 1, len});
                            setSelectEndRC((RCNode) {currentCursor.row - 1, len});
                        }
                    }
                } else {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.row == 1) {
                        setCursorRC((RCNode) {1, 1});
                        setSelectEndRC((RCNode) {1, 1});
                    } else {
                        int len = getRowLen(currentCursor.row - 1);
                        if (currentCursor.column <= len) {
                            setCursorRC((RCNode) {currentCursor.row - 1, currentCursor.column});
                            setSelectEndRC((RCNode) {currentCursor.row - 1, currentCursor.column});
                        } else {
                            setCursorRC((RCNode) {currentCursor.row - 1, len});
                            setSelectEndRC((RCNode) {currentCursor.row - 1, len});
                        }
                    }
                }
                break;
            //������
            case VK_DOWN:
                if (!isShift) {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.row == getTotalRow()) {
                        int len = getRowLen(currentCursor.row);
                        setCursorRC((RCNode) {currentCursor.row, len});
                        setSelectStartRC((RCNode) {currentCursor.row, len});
                        setSelectEndRC((RCNode) {currentCursor.row, len});
                    } else {
                        int len = getRowLen(currentCursor.row + 1);
                        if (currentCursor.column <= len) {
                            setCursorRC((RCNode) {currentCursor.row + 1, currentCursor.column});
                            setSelectStartRC((RCNode) {currentCursor.row + 1, currentCursor.column});
                            setSelectEndRC((RCNode) {currentCursor.row + 1, currentCursor.column});
                        } else {
                            setCursorRC((RCNode) {currentCursor.row + 1, len});
                            setSelectStartRC((RCNode) {currentCursor.row + 1, len});
                            setSelectEndRC((RCNode) {currentCursor.row + 1, len});
                        }
                    }
                } else {
                    RCNode currentCursor = getCursorRC();
                    if (currentCursor.row == getTotalRow()) {
                        int len = getRowLen(currentCursor.row);
                        setCursorRC((RCNode) {currentCursor.row, len});
                        setSelectEndRC((RCNode) {currentCursor.row, len});
                    } else {
                        int len = getRowLen(currentCursor.row + 1);
                        if (currentCursor.column <= len) {
                            setCursorRC((RCNode) {currentCursor.row + 1, currentCursor.column});
                            setSelectEndRC((RCNode) {currentCursor.row + 1, currentCursor.column});
                        } else {
                            setCursorRC((RCNode) {currentCursor.row + 1, len});
                            setSelectEndRC((RCNode) {currentCursor.row + 1, len});
                        }
                    }
                }
                break;
            //END
            case VK_END:
                if (!isShift) {
                    RCNode currentCursor = getCursorRC();
                    int len = getRowLen(currentCursor.row);
                    setCursorRC((RCNode) {currentCursor.row, len});
                    setSelectStartRC((RCNode) {currentCursor.row, len});
                    setSelectEndRC((RCNode) {currentCursor.row, len});
                } else {
                    RCNode currentCursor = getCursorRC();
                    int len = getRowLen(currentCursor.row);
                    setCursorRC((RCNode) {currentCursor.row, len});
                    setSelectEndRC((RCNode) {currentCursor.row, len});
                }
                break;
            //HOME
            case VK_HOME:
                if (!isShift) {
                    RCNode currentCursor = getCursorRC();
                    setCursorRC((RCNode) {currentCursor.row, 1});
                    setSelectStartRC((RCNode) {currentCursor.row, 1});
                    setSelectEndRC((RCNode) {currentCursor.row, 1});
                } else {
                    RCNode currentCursor = getCursorRC();
                    setCursorRC((RCNode) {currentCursor.row, 1});
                    setSelectEndRC((RCNode) {currentCursor.row, 1});
                }
                break;
            //BACKSPACE
            // case VK_BACK:
            //     puts("BACKSPACE");
            //     break;
            //DELETE
            case VK_DELETE:
                puts("DELETE");
                break;
            //TAB
            // case VK_TAB:
            //     puts("TAB");
            //     break;
            //ENTER
            // case VK_RETURN:
            //     puts("ENTER");
            //     break;
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