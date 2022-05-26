/*
    ��Ҫ������� ���̰����Ļص�
*/

//TODO:�������ҷ����ʱ�����ַ��Ĵ���

#include "global.h"
#include "libgraphics.h"
#include "display.h"
#include "file.h"
#include "style.h"
#include "math.h"
#include "imgui.h"
#include "storage.h"
#include "callback.h"
#include "ctype.h"
#include "record.h"

static int isButtonDown = 0;
static int isShift = 0;
static int isTyping = 1;
static int isCtrl = 0;

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
    }

    char *originFont = GetFont();
    int originPointSize = GetPointSize();
    SetFont("΢���ź�");
    SetPointSize(13);

    double fH = GetFontHeight();
    double w = TextStringWidth("�ļ�") * 2;

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
                setCursorInWindow();
            }
            if (getContextMenuDisplayState()) setContextMenuDisplayState(0);    //����ʾ�Ҽ��˵�
            break;
        case MOUSEMOVE:
            if (isButtonDown) {     //����϶�ѡ��
                RCNode mouse = XYtoRC(x, y);
                setCursorRC(mouse);
                setSelectEndRC(mouse);
                setCursorInWindow();
            }
            break;
        case BUTTON_UP:
            if (button == LEFT_BUTTON) {
                // RCNode mouse = XYtoRC(x, y);
                isButtonDown = 0;
                // setCursorRC(mouse);
                // setSelectEndRC(mouse);
                // setCursorInWindow();
            } else if (button == RIGHT_BUTTON && getTextDisplayState()) {
                if (getContextMenuDisplayState()) {
                    setContextMenuDisplayState(0);
                    isTyping = 1;
                } else {
                    setContextMenuXY(nx, ny);
                    setContextMenuDisplayState(1);
                    isTyping = 0;
                }
            }
            break;
        case ROLL_UP: 
            if (!isShift) {     //���ϻ���
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {max(winCurrent.row - (ceil(winWidth / 12 / h)), 1),    //��ֹ���������滹�����ϻ�
                                                winCurrent.column});
            } else {    //���󻬶�
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {winCurrent.row,
                                                max(winCurrent.column - (ceil(winHeight / 12 / TextStringWidth(" "))), 1)});    //��ֹ��������߼�������
            }
            break;
        case ROLL_DOWN:
            if (!isShift) {     //���»���
                RCNode winCurrent = getWindowCurrentRC();
                int totr = getTotalRow();
                setWindowCurrentRC((RCNode) {min(winCurrent.row + (ceil(winWidth / 12 / h)), totr),     //��ֹ����������������»�
                                                winCurrent.column});
            } else {    //���һ���
                RCNode winCurrent = getWindowCurrentRC();
                setWindowCurrentRC((RCNode) {winCurrent.row, winCurrent.column + (ceil(winHeight / 12 / TextStringWidth(" ")))});
            }
            break;
        case BUTTON_DOUBLECLICK:    //���˫��
            if (button == LEFT_BUTTON) {   
                RCNode mouse = XYtoRC(x, y);
                RCNode start = mouse, end = mouse;
                const string s = getRowContent(mouse.row);
                char ch = s[mouse.column - 1];
                int len = getRowLength(mouse.row);
                if (isalnum(ch)) {  //���������/��ĸ����ѡ����������������/��ĸ�������»��ߣ�
                    while (start.column >= 1 && (isalnum(s[start.column - 1]) || s[start.column - 1] == '_')) {
                        start.column--;
                    }
                    start.column++;
                    while (end.column <= len && (isalnum(s[end.column - 1]) || s[end.column - 1] == '_')) {
                        end.column++;
                    }
                    setSelectStartRC(start);
                    setSelectEndRC(end);
                    setCursorRC(end);
                    setCursorInWindow();
                } else if (ch & 0x80) { //��������ģ���ѡ����������������
                    while (start.column >= 1 && (s[start.column - 1] & 0x80)) {
                        start.column--;
                    }
                    start.column++;
                    while (end.column <= len && (s[end.column - 1] & 0x80)) {
                        end.column++;
                    }
                    setSelectStartRC(start);
                    setSelectEndRC(end);
                    setCursorRC(end);
                    setCursorInWindow();
                } else if (isspace(ch)) {     //����ǿո�/�Ʊ������ѡ�����������пո�/�Ʊ��
                    while (start.column >= 1 && isspace(s[start.column - 1])) {
                        start.column--;
                    }
                    start.column++;
                    while (end.column <= len && isspace(s[end.column - 1])) {
                        end.column++;
                    }
                    setSelectStartRC(start);
                    setSelectEndRC(end);
                    setCursorRC(end);
                    setCursorInWindow();
                } else if (ch == '_') {     //������»��ߣ�����Ա�������/��ĸ���������/��ĸ������������������ͨ������
                    while (start.column >= 1 && (isalnum(s[start.column - 1]) || s[start.column - 1] == '_')) {
                        start.column--;
                    }
                    start.column++;
                    while (end.column <= len && (isalnum(s[end.column - 1]) || s[end.column - 1] == '_')) {
                        end.column++;
                    }
                    if (start.column == mouse.column && end.column == mouse.column + 1) {   //�Ա�û������/��ĸ
                        while (start.column >= 1 && ispunct(s[start.column - 1])) {
                            start.column--;
                        }
                        start.column++;
                        while (end.column <= len && ispunct(s[end.column - 1])) {
                            end.column++;
                        }
                    }
                    setSelectStartRC(start);
                    setSelectEndRC(end);
                    setCursorRC(end);
                    setCursorInWindow();
                } else if (ispunct(ch)) {   //����Ǳ����ţ���ѡ�����������ı�����
                    while (start.column >= 1 && ispunct(s[start.column - 1])) {
                        start.column--;
                    }
                    start.column++;
                    while (end.column <= len && ispunct(s[end.column - 1])) {
                        end.column++;
                    }
                    setSelectStartRC(start);
                    setSelectEndRC(end);
                    setCursorRC(end);
                    setCursorInWindow();
                }
            }
            break;

    }

    SetFont(originFont);
    SetPointSize(originPointSize);
}

static char chinese[3];     //���һ�������ַ�
static char lastChar = 0;   //��һ��������ַ�

/*
    ���ݴ�����ַ������еȣ�ʵ�ֽ������������ӣ����룩����ǰ�ļ���ĳ��ĳ��(r, c)�Ĺ��ܡ�
*/
void inputChar(char ch) {
    // printf("INPUT:%d\n", ch);
    if (!isTyping) return;
    if (ch > 0 && ch < 32) {
        return;
    }   //���������ַ�
    setContextMenuDisplayState(0);
    setSaveState(0);    //�²���δ����
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
    int recordID = newRecordID();
    if (!(startSelect.row == endSelect.row && startSelect.column == endSelect.column)) {    //�����ѡ�з�Χ���Ȱ�ѡ�з�Χ������ɾ��
        deleteContent(startSelect, endSelect, recordID);
        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {   //�����ʼ�ڽ���֮���򽻻�˳��
            RCNode t = startSelect;
            startSelect = endSelect;
            endSelect = t;
        }
        setSelectStartRC(startSelect);
        setSelectEndRC(startSelect);
        setCursorRC(startSelect);
    }
    RCNode cursor = getCursorRC();
    if (ch & 0x80) {     //�����ַ�
        if (!lastChar) {    //����������ַ��ĵ�һ��ch�򱣴�����
            lastChar = ch;
        } else {    //������������ַ��ĵڶ���ch����ӵ�������
            chinese[0] = lastChar;
            chinese[1] = ch;
            chinese[2] = 0;
            lastChar = 0;
            addContentByString(cursor, chinese, recordID);
            cursor.column += 2;
            setCursorRC(cursor);
            setSelectStartRC(cursor);
            setSelectEndRC(cursor);
            lastChar = 0;
        }
    } else {    //һ���ַ�
        addContentByChar(cursor, ch, recordID);
        cursor.column++;
        setCursorRC(cursor);
        setSelectStartRC(cursor);
        setSelectEndRC(cursor);
        setCursorInWindow();
        lastChar = 0;
    }
}

/*
    ���ݴ���ļ������룬�ж��Ƿ���Ҫ���С��˸�Ȳ���
    �������Ҽ�ʱ�ƶ����λ�ã����ı���������
    Shift+�����ʱ�ı�ѡ��Χ�����ѹ��ŵ�ѡ��Χ��ͷ
*/
void inputKeyboard(int key, int event) {
    // printf("KB:%d %d\n", key, event);
    if (event == KEY_DOWN) {
        switch (key) {
            //������
            case VK_LEFT: {
                RCNode cursor = getCursorRC();
                RCNode startSelect = getSelectStartRC();
                RCNode endSelect = getSelectEndRC();
                if ((startSelect.column != endSelect.column || startSelect.row != endSelect.row) && !isShift) {   //��ǰ��ѡ�з�Χ����ѹ���ƶ���ѡ�з�Χǰ��
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
                if (cursor.column == 1) {   //�����ǰ�ڵ�һ�У��Ҳ��ڵ�һ�У���Ҫ�ѹ��Ų����һ��ĩβ
                    if (cursor.row > 1) {
                        int len = getRowLength(cursor.row - 1);
                        setCursorRC((RCNode) {cursor.row - 1, len});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row - 1, len});     //�������shift����ѡȡ״̬�������������ÿ�ʼѡ��Χ��λ��
                        setSelectEndRC((RCNode) {cursor.row - 1, len});
                    }
                } else {
                    string s = getRowContent(cursor.row);
                    if (cursor.column >= 2 && s[cursor.column - 2] & 0x80) {  //�����֣�����Ǻ���Ҫ����������
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
            //������
            case VK_RIGHT: {
                RCNode cursor = getCursorRC();
                RCNode startSelect = getSelectStartRC();
                RCNode endSelect = getSelectEndRC();
                if ((startSelect.column != endSelect.column || startSelect.row != endSelect.row) && !isShift) {   //��ǰ��ѡ�з�Χ��û�а���shift����ѹ���ƶ���ѡ�з�Χĩ��
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
                if (cursor.column == rowLen + 1) {    //��������ĳ�е����һ�У��Ҳ������һ�У���ҪŲ����һ�п�ͷ
                    if (cursor.row < getTotalRow()) {
                        setCursorRC((RCNode) {cursor.row + 1, 1});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row + 1, 1});
                        setSelectEndRC((RCNode) {cursor.row + 1, 1});
                    }
                    //���������ĩβ�򲻲���
                } else {
                    if (s[cursor.column - 1] & 0x80) {  //�����֣�����Ǻ���Ҫ����������
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
            //������
            case VK_UP: {
                RCNode cursor = getCursorRC();
                if (cursor.row == 1) {   //��ǰ����ڵ�һ�У���ѹ��Ų�����¿�ͷ
                    setCursorRC((RCNode) {1, 1});
                    if (!isShift) setSelectStartRC((RCNode) {1, 1});
                    setSelectEndRC((RCNode) {1, 1});
                } else {
                    string s = getRowContent(cursor.row - 1);
                    int i = 0, lens = getRowLength(cursor.row - 1);
                    //Ϊ�˷�ֹ�������������ַ����м䣬Ҫ�����ж�
                    int flag = 0;
                    while (i < lens && s[i] != '\n') {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //������Ҫ���������ַ����м䣬��Ӧ�ð����ŵ��������ַ���ǰ��
                            cursor.row--;
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //��������
                        i++;
                    }
                    if (!flag) {    //��������һ�е����м䣬��ѹ���Ƶ���һ�е���ĩ
                        setCursorRC((RCNode) {cursor.row - 1, lens});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row - 1, lens});
                        setSelectEndRC((RCNode) {cursor.row - 1, lens});
                    }
                }
                setCursorInWindow();
                break;
            }
            //������
            case VK_DOWN: {
                RCNode cursor = getCursorRC();
                if (cursor.row == getTotalRow()) {   //�����������һ�У����ƶ�������ĩβ
                    int len = getRowLength(cursor.row);
                    setCursorRC((RCNode) {cursor.row, len + 1});
                    if (!isShift) setSelectStartRC((RCNode) {cursor.row, len + 1});
                    setSelectEndRC((RCNode) {cursor.row, len + 1});
                } else {
                    string s = getRowContent(cursor.row + 1);
                    int i = 0, lens = getRowLength(cursor.row + 1);
                    if (lens && s[lens - 1] == '\n') lens--;    //��lens��ȥ��ĩ���з�
                    //Ϊ�˷�ֹ�������������ַ����м䣬Ҫ�����ж�
                    int flag = 0;
                    while (i < lens) {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //������Ҫ���������ַ����м䣬��Ӧ�ð����ŵ��������ַ���ǰ��
                            cursor.row++;
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //��������
                        i++;
                    }
                    if (!flag) {    //��������һ�е����м䣬��ѹ���Ƶ���һ�е���ĩ
                        setCursorRC((RCNode) {cursor.row + 1, lens + 1});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row + 1, lens + 1});
                        setSelectEndRC((RCNode) {cursor.row + 1, lens + 1});
                    }
                }
                setCursorInWindow();
                break;
            }
            //END
            case VK_END: {  //�ѹ��Ų��������ĩβ
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
            case VK_HOME: {     //�ѹ��Ų�����п�ͷ
                RCNode cursor = getCursorRC();
                setCursorRC((RCNode) {cursor.row, 1});
                if (!isShift) setSelectStartRC((RCNode) {cursor.row, 1});
                setSelectEndRC((RCNode) {cursor.row, 1});
                
                setCursorInWindow();
                break;
            }
            //BACKSPACE
            case VK_BACK:   //ɾ�����ǰ��һ���ַ�
                if (isTyping) {
                    RCNode cursor = getCursorRC();
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    if (!(startSelect.column == endSelect.column && startSelect.row == endSelect.row)) {    //�����ѡ�о�ɾ��ѡ�����ݣ����ٲ���
                        setSaveState(0);    //�²���δ����
                        deleteContent(startSelect, endSelect, newRecordID());
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setCursorRC(startSelect);
                        setSelectEndRC(startSelect);
                        return;
                    }
                    if (cursor.row == 1 && cursor.column == 1) break;   //�����������¿�ͷ�򲻲���
                    string s = getRowContent(cursor.row);
                    setSaveState(0);    //�²���δ����
                    if (cursor.column == 1) {   //����������ͷ����ɾ�����з������кϲ���һ��
                        cursor.row--;
                        cursor.column = getRowLength(cursor.row);
                        deleteContent(cursor, (RCNode) {cursor.row, cursor.column + 1}, newRecordID());
                        setCursorRC(cursor);
                        setSelectStartRC(cursor);
                        setSelectEndRC(cursor);
                    } else {
                        if (cursor.column >= 3 && s[cursor.column - 2] & 0x80) {    //���Ҫɾ����������
                            deleteContent((RCNode) {cursor.row, cursor.column - 2}, cursor, newRecordID());
                            cursor.column -= 2;
                        } else {
                            deleteContent((RCNode) {cursor.row, cursor.column - 1}, cursor, newRecordID());
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
            case VK_DELETE:     //ɾ���������λ�õ��ַ�
                if (isTyping) {
                    RCNode cursor = getCursorRC();
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    if (!(startSelect.column == endSelect.column && startSelect.row == endSelect.row)) {    //�����ѡ�о�ɾ��ѡ�����ݣ����ٲ���
                        setSaveState(0);    //�²���δ����
                        deleteContent(startSelect, endSelect, newRecordID());
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setCursorRC(startSelect);
                        setSelectEndRC(startSelect);
                        return;
                    }
                    if (cursor.row == getTotalRow() && cursor.column == getRowLength(cursor.row) + 1) break;    //���������ĩβ�򲻲���
                    setSaveState(0);    //�²���δ����
                    string s = getRowContent(cursor.row);
                    if (s[cursor.column - 1] & 0x80) {      //�����������Ҫɾ�����ַ�
                        deleteContent(cursor, (RCNode) {cursor.row, cursor.column + 2}, newRecordID());
                    } else {
                        deleteContent(cursor, (RCNode) {cursor.row, cursor.column + 1}, newRecordID());
                    }

                    setCursorRC(cursor);
                    setSelectStartRC(cursor);
                    setSelectEndRC(cursor);
                    setCursorInWindow();
                }
                break;
            case VK_ESCAPE:     //�˳�����/�滻�Ĵ���
                if (getFindDisplayState()) setFindDisplayState(0);
                if (getReplaceDisplayState()) setReplaceDisplayState(0);
                break;
            case VK_PRIOR: {    //PageUp����
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
                    //Ϊ�˷�ֹ�������������ַ����м䣬Ҫ�����ж�
                    int flag = 0;
                    while (i < lens && s[i] != '\n') {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //������Ҫ���������ַ����м䣬��Ӧ�ð����ŵ��������ַ���ǰ��
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //��������
                        i++;
                    }
                    if (!flag) {    //��������һ�е����м䣬��ѹ���Ƶ���һ�е���ĩ
                        setCursorRC((RCNode) {cursor.row, lens});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, lens});
                        setSelectEndRC((RCNode) {cursor.row, lens});
                    }
                }
                setCursorInWindow();
                break;
            }
            case VK_NEXT: {     //PageDown����
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
                    //Ϊ�˷�ֹ�������������ַ����м䣬Ҫ�����ж�
                    int flag = 0;
                    while (i < lens && s[i] != '\n') {
                        if (cursor.column == i + 1 || (cursor.column == i + 2 && s[i] & 0x80)) {    //������Ҫ���������ַ����м䣬��Ӧ�ð����ŵ��������ַ���ǰ��
                            cursor.column = i + 1;
                            setCursorRC(cursor);
                            if (!isShift) setSelectStartRC(cursor);
                            setSelectEndRC(cursor);
                            flag = 1;
                            break;
                        }
                        if (s[i] & 0x80) i++;   //��������
                        i++;
                    }
                    if (!flag) {    //��������һ�е����м䣬��ѹ���Ƶ���һ�е���ĩ
                        setCursorRC((RCNode) {cursor.row, lens});
                        if (!isShift) setSelectStartRC((RCNode) {cursor.row, lens});
                        setSelectEndRC((RCNode) {cursor.row, lens});
                    }
                }
                setCursorInWindow();
                break;
            }
            case VK_CONTROL:    //Ctrl
                isCtrl = 1;
                break;
            case VK_RETURN:     //�س�
                if (isTyping) {
                    setSaveState(0);    //�²���δ����
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    int recordID = newRecordID();
                    if (!(startSelect.row == endSelect.row && startSelect.column == endSelect.column)) {    //�����ѡ�з�Χ���Ȱ�ѡ�з�Χ������ɾ��
                        deleteContent(startSelect, endSelect, recordID);
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {   //�����ʼ�ڽ���֮���򽻻�˳��
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setSelectStartRC(startSelect);
                        setSelectEndRC(startSelect);
                        setCursorRC(startSelect);
                    }
                    RCNode cursor = getCursorRC();
                    addContentByChar(cursor, '\n', recordID);  //������'\n��������
                    cursor.row++;
                    cursor.column = 1;
                    setCursorRC(cursor);
                    setSelectStartRC(cursor);
                    setSelectEndRC(cursor);
                    setCursorInWindow();
                    lastChar = 0;
                }
                break;
            case VK_TAB:    //�Ʊ��
                if (isTyping) {
                    setSaveState(0);    //�²���δ����
                    RCNode startSelect = getSelectStartRC();
                    RCNode endSelect = getSelectEndRC();
                    int recordID =  newRecordID();
                    if (!(startSelect.row == endSelect.row && startSelect.column == endSelect.column)) {    //�����ѡ�з�Χ���Ȱ�ѡ�з�Χ������ɾ��
                        deleteContent(startSelect, endSelect, recordID);
                        if (startSelect.row > endSelect.row || (startSelect.row == endSelect.row && startSelect.column > endSelect.column)) {   //�����ʼ�ڽ���֮���򽻻�˳��
                            RCNode t = startSelect;
                            startSelect = endSelect;
                            endSelect = t;
                        }
                        setSelectStartRC(startSelect);
                        setSelectEndRC(startSelect);
                        setCursorRC(startSelect);
                    }
                    RCNode cursor = getCursorRC();
                    addContentByChar(cursor, '\t', recordID);  //������'\n��������
                    cursor.column++;
                    setCursorRC(cursor);
                    setSelectStartRC(cursor);
                    setSelectEndRC(cursor);
                    setCursorInWindow();
                    lastChar = 0;
                }
                break;
        }
        processShortcutKey(key, isShift, isCtrl, isTyping);
    }
    if (event == KEY_UP) {
        if (key == VK_SHIFT) {
            isShift = 0;
        } else if (key == VK_CONTROL) {
            isCtrl = 0;
        }
    }
}