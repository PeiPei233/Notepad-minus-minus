/*
    主要处理鼠标 键盘按键的回调
*/

#include "global.h"
#include "libgraphics.h"
#include "display.h"
#include "file.h"

int isButtonDown = 0;
int isShift = 0;
static char t[100010];

/**
 * 获得第 row 行的总列数
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
 * 获得文件总行数
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
 * 将窗口的(x, y)像素坐标转换为行列坐标
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
    自定义传入参数（如当前坐标，文字绘制范围，行高等） 实现根据当前的(x, y)坐标返回在文件中的行列坐标(r, c)
    r: row 行
    c: column 列
    鼠标滚动时更改当前显示的文本范围左上角行列位置
    左键按下并拖动时进行选择
    选择后光标位置应该在选择范围的最后
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
    根据传入的字符，行列等，实现将输入的文字添加（插入）到当前文件中某行某列(r, c)的功能。
*/
void inputChar(char ch) {
    printf("INPUT:%d\n", ch);
}

/*
    根据传入的键盘输入，判断是否需要换行、退格等操作
    上下左右键时移动光标位置（即改变行列数）
    Shift+方向键时改变选择范围，并把光标放到选择范围开头
*/
void inputKeyboard(int key, int event) {
    if (event == KEY_DOWN) {
        switch (key) {
            //方向左
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
            //方向右
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
            //方向上
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
            //方向下
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