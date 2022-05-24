/*
    主要处理鼠标 键盘按键的回调
*/

//TODO:上下左右方向键时中文字符的处理

#include "global.h"
#include "libgraphics.h"
#include "display.h"
#include "file.h"
#include "style.h"
#include "math.h"
#include "imgui.h"
#include "storage.h"
#include "callback.h"

static int isButtonDown = 0;
static int isShift = 0;
static int isTyping = 1;
static int isCtrl = 0;

/**
 * 获取当前输入状态
 */
int getTypingState() {
    return isTyping;
}

/**
 * 设置当前输入状态
 */ 
void setTypingState(int newTypingState) {
    isTyping = newTypingState;
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

    double nx = ScaleXInches(x);
    double ny = ScaleYInches(y);

    UIState gs_UIState = getUIState();
    // printf("CLICKITEM:%d\n", gs_UIState.clickedItem);

    if (gs_UIState.clickedItem) {
        isTyping = 0;
        return;
    }

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("微软雅黑");
    SetPointSize(13);

    double fH = GetFontHeight();
    double w = TextStringWidth("文件") * 2;

    SetFont(originFont);
    SetPointSize(originPointSize);

    if (button == LEFT_BUTTON && event == BUTTON_DOWN && getContextMenuDisplayState()) setContextMenuDisplayState(0);
    
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
                setCursorInWindow();
            }
            break;
        case BUTTON_UP:
            if (button == LEFT_BUTTON) {
                RCNode mouse = XYtoRC(x, y);
                isButtonDown = 0;
                setCursorRC(mouse);
                setSelectEndRC(mouse);
                setCursorInWindow();
            } else if (button == RIGHT_BUTTON && getTextDisplayState()) {
                if (getContextMenuDisplayState()) {
                    setContextMenuDisplayState(0);
                } else {
                    setContextMenuXY(nx, ny);
                    setContextMenuDisplayState(1);
                }
            }
            break;
        case ROLL_UP: 
            if (!isShift) {     //往上滑动
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {max(winCurrent.row - (ceil(winWidth / 12 / h)), 1),    //防止划到最上面还在往上滑
                                                winCurrent.column});
            } else {    //往左滑动
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {winCurrent.row,
                                                max(winCurrent.column - (ceil(winHeight / 12 / TextStringWidth(" "))), 1)});    //防止划到最左边继续往左滑
            }
            break;
        case ROLL_DOWN:
            if (!isShift) {     //往下滑动
                RCNode winCurrent = getWindowCurrentRC();
                int totr = getTotalRow();
                setWindowCurrentRC((RCNode) {min(winCurrent.row + (ceil(winWidth / 12 / h)), totr),     //防止划到最下面继续往下滑
                                                winCurrent.column});
            } else {    //往右滑动
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

static char chinese[3];     //存放一个中文字符
static char lastChar = 0;   //上一个读入的字符

/*
    根据传入的字符，行列等，实现将输入的文字添加（插入）到当前文件中某行某列(r, c)的功能。
*/
void inputChar(char ch) {
    // printf("INPUT:%d\n", ch);
    if (!isTyping) return;
    if (ch == 23) {     //Ctrl + W 退出
        exitApplication();
        return;
    }
    if (ch > 0 && ch < 32 && ch != '\t' && ch != '\r') {
        return;
    }   //跳过控制字符
    setSaveState(0);    //新操作未保存
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    if (!(startSelect.row == endSelect.row && startSelect.column == endSelect.column)) {    //如果有选中范围则先把选中范围的内容删掉
        deleteContent(startSelect, endSelect, 1);
        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {   //如果开始在结束之后则交换顺序
            RCNode t = startSelect;
            startSelect = endSelect;
            endSelect = t;
        }
        setSelectStartRC(startSelect);
        setSelectEndRC(startSelect);
        setCursorRC(startSelect);
    }
    RCNode cursor = getCursorRC();
    if (ch == '\r') {   //回车的键盘回调字符为'\r'
        ch = '\n';  //换行用'\n‘来储存
        addContentByChar(cursor, ch, 1);
        cursor.row++;
        cursor.column = 1;
        setCursorRC(cursor);
        setSelectStartRC(cursor);
        setSelectEndRC(cursor);
        lastChar = 0;
    } else if (ch & 0x80) {     //中文字符
        if (!lastChar) {    //输入的中文字符的第一个ch则保存下来
            lastChar = ch;
        } else {    //输入的是中文字符的第二个ch则添加到数据中
            chinese[0] = lastChar;
            chinese[1] = ch;
            chinese[2] = 0;
            lastChar = 0;
            addContentByString(cursor, chinese, 1);
            cursor.column += 2;
            setCursorRC(cursor);
            setSelectStartRC(cursor);
            setSelectEndRC(cursor);
            lastChar = 0;
        }
    } else {    //一般字符
        addContentByChar(cursor, ch, 1);
        cursor.column++;
        setCursorRC(cursor);
        setSelectStartRC(cursor);
        setSelectEndRC(cursor);
        setCursorInWindow();
        lastChar = 0;
    }
}

/*
    根据传入的键盘输入，判断是否需要换行、退格等操作
    上下左右键时移动光标位置（即改变行列数）
    Shift+方向键时改变选择范围，并把光标放到选择范围开头
*/
void inputKeyboard(int key, int event) {
    // printf("KB:%d %d\n", key, event);
    if (event == KEY_DOWN) {
        switch (key) {
            //方向左
            case VK_LEFT: {
                RCNode cursor = getCursorRC();
                RCNode startSelect = getSelectStartRC();
                RCNode endSelect = getSelectEndRC();
                if ((startSelect.column != endSelect.column || startSelect.row != endSelect.row) && !isShift) {   //当前有选中范围，则把光标移动到选中范围前端
                    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                        RCNode t = startSelect;
                        startSelect = endSelect;
                        endSelect = t;
                    }
                    setSelectStartRC(startSelect);
                    setSelectEndRC(startSelect);
                    setCursorRC(startSelect);
                    break;
                }
                if (cursor.column == 1) {   //如果当前在第一列，且不在第一行，则要把光标挪到上一行末尾
                    if (cursor.row > 1) {
                        int len = getRowLength(cursor.row - 1);
                        setCursorRC((RCNode) {cursor.row - 1, len});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row - 1, len});     //如果按下shift则是选取状态，不用重新设置开始选择范围的位置
                        setSelectEndRC((RCNode) {cursor.row - 1, len});
                    }
                } else {
                    string s = getRowContent(cursor.row);
                    if (cursor.column >= 2 && s[cursor.column - 2] & 0x80) {  //处理汉字，如果是汉字要往左移两格
                        setCursorRC((RCNode) {cursor.row, cursor.column - 2});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, cursor.column - 2});
                        setSelectEndRC((RCNode) {cursor.row, cursor.column - 2});
                    } else {
                        setCursorRC((RCNode) {cursor.row, cursor.column - 1});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, cursor.column - 1});
                        setSelectEndRC((RCNode) {cursor.row, cursor.column - 1});
                    }
                }
                setCursorInWindow();
                break;
            }
            //方向右
            case VK_RIGHT: {
                RCNode cursor = getCursorRC();
                RCNode startSelect = getSelectStartRC();
                RCNode endSelect = getSelectEndRC();
                if ((startSelect.column != endSelect.column || startSelect.row != endSelect.row) && !isShift) {   //当前有选中范围且没有按下shift，则把光标移动到选中范围末端
                    if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                        RCNode t = startSelect;
                        startSelect = endSelect;
                        endSelect = t;
                    }
                    setSelectStartRC(endSelect);
                    setSelectEndRC(endSelect);
                    setCursorRC(endSelect);
                    break;
                }
                int rowLen = getRowLength(cursor.row);
                string s = getRowContent(cursor.row);
                if (s[rowLen - 1] == '\n') rowLen--;
                if (cursor.column == rowLen + 1) {    //如果光标在某行的最后一列，且不是最后一行，则要挪到下一行开头
                    if (cursor.row < getTotalRow()) {
                        setCursorRC((RCNode) {cursor.row + 1, 1});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row + 1, 1});
                        setSelectEndRC((RCNode) {cursor.row + 1, 1});
                    }
                    //如果在文章末尾则不操作
                } else {
                    if (s[cursor.column - 1] & 0x80) {  //处理汉字，如果是汉字要往右移两列
                        setCursorRC((RCNode) {cursor.row, cursor.column + 2});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, cursor.column + 2});
                        setSelectEndRC((RCNode) {cursor.row, cursor.column + 2});
                    } else {
                        setCursorRC((RCNode) {cursor.row, cursor.column + 1});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, cursor.column + 1});
                        setSelectEndRC((RCNode) {cursor.row, cursor.column + 1});
                    }
                }
                setCursorInWindow();
                break;
            }
            //SHIFT
            case VK_SHIFT:
                isShift = 1;
                break;
            //方向上
            case VK_UP: {
                RCNode cursor = getCursorRC();
                if (cursor.row == 1) {   //当前光标在第一行，则把光标挪到文章开头
                    setCursorRC((RCNode) {1, 1});
                    if (!isShift) setSelectStartRC((RCNode) {1, 1});
                    setSelectEndRC((RCNode) {1, 1});
                } else {
                    string s = getRowContent(cursor.row - 1);
                    int i = 0, lens = getRowLength(cursor.row - 1);
                    //为了防止光标出现在中文字符的中间，要特殊判断
                    int flag = 0;
                    while (i < lens && s[i] != '\n') {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //如果光标要放在中文字符的中间，则应该把他放到该中文字符的前面
                            cursor.row--;
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //处理中文
                        i++;
                    }
                    if (!flag) {    //若不在上一行的行中间，则把光标移到上一行的行末
                        setCursorRC((RCNode) {cursor.row - 1, lens});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row - 1, lens});
                        setSelectEndRC((RCNode) {cursor.row - 1, lens});
                    }
                }
                setCursorInWindow();
                break;
            }
            //方向下
            case VK_DOWN: {
                RCNode cursor = getCursorRC();
                if (cursor.row == getTotalRow()) {   //如果光标在最后一行，则移动到文章末尾
                    int len = getRowLength(cursor.row);
                    setCursorRC((RCNode) {cursor.row, len + 1});
                    if (!isShift) setSelectStartRC((RCNode) {cursor.row, len + 1});
                    setSelectEndRC((RCNode) {cursor.row, len + 1});
                } else {
                    string s = getRowContent(cursor.row + 1);
                    int i = 0, lens = getRowLength(cursor.row + 1);
                    if (lens && s[lens - 1] == '\n') lens--;    //将lens减去行末换行符
                    //为了防止光标出现在中文字符的中间，要特殊判断
                    int flag = 0;
                    while (i < lens) {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //如果光标要放在中文字符的中间，则应该把他放到该中文字符的前面
                            cursor.row++;
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //处理中文
                        i++;
                    }
                    if (!flag) {    //若不在上一行的行中间，则把光标移到下一行的行末
                        setCursorRC((RCNode) {cursor.row + 1, lens + 1});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row + 1, lens + 1});
                        setSelectEndRC((RCNode) {cursor.row + 1, lens + 1});
                    }
                }
                setCursorInWindow();
                break;
            }
            //END
            case VK_END: {  //把光标挪到该行最末尾
                RCNode cursor = getCursorRC();
                int len = getRowLength(cursor.row);
                if (cursor.row == getTotalRow()) len++;
                setCursorRC((RCNode) {cursor.row, len});
                if (!isShift) setSelectStartRC((RCNode) {cursor.row, len});
                setSelectEndRC((RCNode) {cursor.row, len});

                setCursorInWindow();
                break;
            }
            //HOME
            case VK_HOME: {     //把光标挪到该行开头
                RCNode cursor = getCursorRC();
                setCursorRC((RCNode) {cursor.row, 1});
                if (!isShift) setSelectStartRC((RCNode) {cursor.row, 1});
                setSelectEndRC((RCNode) {cursor.row, 1});
                
                setCursorInWindow();
                break;
            }
            //BACKSPACE
            case VK_BACK:   //删除光标前的一个字符
                if (isTyping) {
                    RCNode cursor = getCursorRC();
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    if (!(startSelect.column == endSelect.column && startSelect.row == endSelect.row)) {    //如果有选中就删除选中内容，不再操作
                        setSaveState(0);    //新操作未保存
                        deleteContent(startSelect, endSelect, 1);
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setCursorRC(startSelect);
                        setSelectEndRC(startSelect);
                        return;
                    }
                    if (cursor.row == 1 && cursor.column == 1) break;   //如果光标在文章开头则不操作
                    string s = getRowContent(cursor.row);
                    setSaveState(0);    //新操作未保存
                    if (cursor.column == 1) {   //如果光标在行头，则删除换行符，两行合并成一行
                        cursor.row--;
                        cursor.column = getRowLength(cursor.row);
                        deleteContent(cursor, (RCNode) {cursor.row, cursor.column + 1}, 1);
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    } else {
                        if (cursor.column >= 3 && s[cursor.column - 2] & 0x80) {    //如果要删除的是中文
                            deleteContent((RCNode) {cursor.row, cursor.column - 2}, cursor, 1);
                            cursor.column -= 2;
                        } else {
                            deleteContent((RCNode) {cursor.row, cursor.column - 1}, cursor, 1);
                            cursor.column -= 1;
                        }
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    }
                    setCursorInWindow();
                }
                break;
            //DELETE
            case VK_DELETE:     //删除光标所在位置的字符
                if (isTyping) {
                    RCNode cursor = getCursorRC();
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    if (!(startSelect.column == endSelect.column && startSelect.row == endSelect.row)) {    //如果有选中就删除选中内容，不再操作
                        setSaveState(0);    //新操作未保存
                        deleteContent(startSelect, endSelect, 1);
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setCursorRC(startSelect);
                        setSelectEndRC(startSelect);
                        return;
                    }
                    if (cursor.row == getTotalRow() && cursor.column == getRowLength(cursor.row) + 1) break;    //光标在文章末尾则不操作
                    setSaveState(0);    //新操作未保存
                    string s = getRowContent(cursor.row);
                    if (s[cursor.column - 1] & 0x80) {      //如果是中文则要删两个字符
                        deleteContent(cursor, (RCNode) {cursor.row, cursor.column + 2}, 1);
                    } else {
                        deleteContent(cursor, (RCNode) {cursor.row, cursor.column + 1}, 1);
                    }

                    setCursorRC(cursor);
                    setSelectStartRC(cursor);
                    setSelectEndRC(cursor);
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
            case VK_ESCAPE:     //退出查找/替换的窗口
                if (getFindDisplayState()) setFindDisplayState(0);
                if (getReplaceDisplayState()) setReplaceDisplayState(0);
                break;
            case VK_PRIOR: {    //PageUp按键
                int totl = getTotalDisplayRow();
                RCNode cursor = getCursorRC();
                if (cursor.row - totl + 1 < 1) {
                    cursor.row = 1;
                    cursor.column = 1;
                    setCursorRC(cursor);
                    if (!isShift) setSelectStartRC(cursor);
                    setSelectEndRC(cursor);
                } else {
                    cursor.row -= totl - 1;
                    string s = getRowContent(cursor.row);
                    int i = 0, lens = getRowLength(cursor.row);
                    //为了防止光标出现在中文字符的中间，要特殊判断
                    int flag = 0;
                    while (i < lens && s[i] != '\n') {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //如果光标要放在中文字符的中间，则应该把他放到该中文字符的前面
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //处理中文
                        i++;
                    }
                    if (!flag) {    //若不在上一行的行中间，则把光标移到上一行的行末
                        setCursorRC((RCNode) {cursor.row, lens});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, lens});
                        setSelectEndRC((RCNode) {cursor.row, lens});
                    }
                }
                setCursorInWindow();
                break;
            }
            case VK_NEXT: {     //PageDown按键
                int totl = getTotalDisplayRow();
                RCNode cursor = getCursorRC();
                if (cursor.row + totl - 1 > getTotalRow()) {
                    cursor.row = getTotalRow();
                    int len = getRowLength(cursor.row);
                    setCursorRC((RCNode) {cursor.row, len + 1});
                    if (!isShift) setSelectStartRC((RCNode) {cursor.row, len + 1});
                    setSelectEndRC((RCNode) {cursor.row, len + 1});
                } else {
                    cursor.row += totl - 1;
                    string s = getRowContent(cursor.row);
                    int i = 0, lens = getRowLength(cursor.row);
                    //为了防止光标出现在中文字符的中间，要特殊判断
                    int flag = 0;
                    while (i < lens && s[i] != '\n') {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //如果光标要放在中文字符的中间，则应该把他放到该中文字符的前面
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //处理中文
                        i++;
                    }
                    if (!flag) {    //若不在上一行的行中间，则把光标移到上一行的行末
                        setCursorRC((RCNode) {cursor.row, lens});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, lens});
                        setSelectEndRC((RCNode) {cursor.row, lens});
                    }
                }
                setCursorInWindow();
                break;
            }
            case VK_CONTROL:
                isCtrl = 1;
                break;

        }
    }
    if (event == KEY_UP) {
        if (key == VK_SHIFT) {
            isShift = 0;
        } else if (key == VK_CONTROL) {
            isCtrl = 0;
        }
    }
}