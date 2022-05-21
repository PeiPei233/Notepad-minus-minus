/**
 * 
 * 该文件用于处理字符串数组的储存操作
 * 
 */ 

#include "storage.h"
#include "unredo.h"
#include <string.h>

unsigned int sizeR;      //列方向所用总数
unsigned int capR;       //列方向可承载总数
unsigned int *sizeL;     //行方向所用总数
unsigned int *capL;      //行方向可承载总数
char **str;

/**
 * 初始化存储
 */
void initStorage() {
    sizeR = 1;
    capR = 1;
    sizeL = (unsigned int *) malloc(sizeof(unsigned int));
    capL = (unsigned int *) malloc(sizeof(unsigned int));
    str = (char **) malloc(sizeof(char *));
    sizeL[0] = 1;
    capL[0] = 1;
    str[0] = (char *) malloc(sizeof(char));
    str[0][0] = 0;
}

/**
 * 获得某一行的字符串
 */ 
string getRowString(int row) {
    if (row <= sizeR && row >= 1) return str[row - 1];
    else return NULL;
}

/**
 * 获得总行数
 */ 
unsigned int getTotalRow() {
    return sizeR;
}

/**
 * 获得选中范围的字符串的长度
 * 注意获得的字符串长度范围为[start, end)左闭右开区间
 */
unsigned int getSelectStringLength(RCNode start, RCNode end) {
    if (start.row > end.row || (start.row == end.row && start.column > end.column)) {
        RCNode t = start;
        start = end;
        end = t;
    }
    if (start.row == end.row && start.column == end.column) return 0;
    if (start.row == end.row) {
        return end.column - start.column;
    }
    unsigned int len = 0;
    len += sizeL[start.row - 1] - start.column;
    len += end.column - 1;
    for (int i = start.row; i < end.row - 1; i++) {
        len += sizeL[i] - 1;
    }
    return len;
}

/**
 * 获得选中范围的字符串
 * 用完记得及时free
 * 注意获得的字符串范围为[start, end)左闭右开区间
 * 空字符串则返回NULL
 */ 
string getSelectString(RCNode start, RCNode end) {
    if (start.row > end.row || (start.row == end.row && start.column > end.column)) {
        RCNode t = start;
        start = end;
        end = t;
    }
    unsigned int len = getSelectStringLength(start, end);
    if (!len) return NULL;
    char *res = (char *) malloc(sizeof(char) * (len + 1));
    if (start.row == end.row) {
        for (int i = 0; i < len; i++) {
            res[i] = str[start.row - 1][start.column - 1 + i];
        }
        res[len] = 0;
        return res;
    } else {
        int i = 0;
        for (int j = start.column - 1; j < sizeL[start.row - 1] - 1; i++, j++) {
            res[i] = str[start.row - 1][j];
        }
        for (int k = start.row; k < end.row - 1; k++) {
            for (int j = 0; j < sizeL[k] - 1; i++, j++) {
                res[i] = str[k][j];
            }
        }
        for (int j = 0; j < end.column - 1; i++, j++) {
            res[i] = str[end.row - 1][j];
        }
        res[len] = 0;
        return res;
    }
}

/**
 * 获得某一行的长度
 */ 
unsigned int getRowLen(int row) {
    if (row <= sizeR && row >= 1) return sizeL[row - 1] - 1;     //每一行的大小包括最后的'\0' 要减掉
    else return 0;
}

/**
 * 添加字符
 * pos:添加的位置
 * ch:添加的字符
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addChar(RCNode pos, char ch, int doRecord) {
    int row = pos.row - 1, col = pos.column - 1;    //在数组中下标从零开始
    if (doRecord) {
        record(OP_ADD, pos, CharToString(ch));
    }
    if (ch == '\n') {   //若是换行符，则新开一行，其他行下移
        sizeR++;
        if (sizeR > capR) { //列方向装不下了，重开
            capR <<= 1;
            char **tmpStr = (char **) malloc(sizeof(char *) * capR);
            unsigned int *tmpSizeL = (int *) malloc(sizeof(unsigned int) * capR);
            unsigned int *tmpCapL = (int *) malloc(sizeof(unsigned int) * capR);
            //搬运操作，并留下所需的数据
            for (int i = 0; i <= row; i++) {
                tmpStr[i] = str[i];
                tmpSizeL[i] = sizeL[i];
                tmpCapL[i] = capL[i];
            }
            for (int i = row + 2; i < sizeR; i++) {
                tmpStr[i] = str[i - 1];
                tmpSizeL[i] = sizeL[i - 1];
                tmpCapL[i] = capL[i - 1];
            }
            free(str);
            free(sizeL);
            free(capL);
            str = tmpStr;
            sizeL = tmpSizeL;
            capL = tmpCapL;
        } else {
            for (int i = sizeR - 1; i >= row + 2; i--) {
                str[i] = str[i - 1];
                sizeL[i] = sizeL[i - 1];
                capL[i] = capL[i - 1];
            }
        }
        sizeL[row + 1] = sizeL[row] - col;
        capL[row + 1] = 1;
        while (capL[row + 1] < sizeL[row + 1]) {
            capL[row + 1] <<= 1;
        }
        str[row + 1] = (char *) malloc(sizeof(char) * capL[row + 1]);
        for (int i = 0; i < sizeL[row + 1]; i++) {
            str[row + 1][i] = str[row][col + i];
        }
        str[row][col] = '\n';
        str[row][col + 1] = 0;
        sizeL[row] = col + 2;
    } else {    //普通字符则该位置之后的列右移一格
        sizeL[row]++;
        if (sizeL[row] > capL[row]) {   //装不下了，重开
            capL[row] <<= 1;
            char *t = (char *) malloc(sizeof(char) * capL[row]);
            //搬运数据
            for (int i = 0; i < col; i++) {
                t[i] = str[row][i];
            }
            t[col] = ch;
            for (int i = col + 1; i < sizeL[row]; i++) {
                t[i] = str[row][i - 1];
            }
            free(str[row]);
            str[row] = t;
        } else {
            for (int i = sizeL[row] - 1; i > col; i--) {
                str[row][i] = str[row][i - 1];
            }
            str[row][col] = ch;
        }
    }
}

/**
 * 添加字符串，并记录操作
 * start:起始位置
 * src:添加的字符串
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void addString(RCNode start, string src, int doRecord) {
    unsigned int lens = strlen(src);
    if (!lens || !src) return;
    if (doRecord) {
        record(OP_ADD, start, src);
    }

    int row = start.row - 1, col = start.column - 1;

    //计算换行符的个数
    unsigned int cntl = 0;
    for (int i = 0; i < lens; i++) {
        if (src[i] == '\n') cntl++;
    }
    if (cntl) {     //有换行
        sizeR += cntl;  //每个换行符都要开新的一行
        if (sizeR > capR) {     //装不下了，重新开
            while (sizeR > capR) {
                capR <<= 1;
            }
            char **tmpStr = (char **) malloc(sizeof(char *) * capR);
            unsigned int *tmpSizeL = (int *) malloc(sizeof(unsigned int) * capR);
            unsigned int *tmpCapL = (int *) malloc(sizeof(unsigned int) * capR);
            for (int i = 0; i <= row; i++) {
                tmpStr[i] = str[i];
                tmpSizeL[i] = sizeL[i];
                tmpCapL[i] = capL[i];
            }
            for (int i = sizeR - 1; i > row + cntl; i--) {
                tmpStr[i] = str[i - cntl];
                tmpSizeL[i] = sizeL[i - cntl];
                tmpCapL[i] = capL[i - cntl];
            }
            free(str);
            free(sizeL);
            free(capL);
            str = tmpStr;
            sizeL = tmpSizeL;
            capL = tmpCapL;
        } else {
            for (int i = sizeR - 1; i > row + cntl; i--) {
                str[i] = str[i - cntl];
                sizeL[i] = sizeL[i - cntl];
                capL[i] = capL[i - cntl];
            }
        }
        //处理第 row + cntl 行，为 src 最后一行与第 row 行后半段的拼接
        unsigned int len = 0;
        while (src[lens - len - 1] != '\n' && len < lens) len++;    //最后一行的长度
        sizeL[row + cntl] = sizeL[row] - col + len;
        capL[row + cntl] = 1;
        while (capL[row + cntl] < sizeL[row + cntl]) {
            capL[row + cntl] <<= 1;
        }
        str[row + cntl] = (char *) malloc(sizeof(char) * capL[row + cntl]);
        for (int i = 0; i < len; i++) {
            str[row + cntl][i] = src[lens - len + i];
        }
        for (int i = len; i < sizeL[row + cntl]; i++) {
            str[row + cntl][i] = str[row][col + i - len];
        }
        //处理第 row 行，为第 row 行前半段与 src 第一行的拼接
        len = 0;
        while (src[len] != '\n' && len < lens) len++;   //src 第一行到'\n'前的长度
        sizeL[row] = col + len + 2;
        if (sizeL[row] > capL[row]) {   //装不下了，重开，并将需要的数据留下来
            while (sizeL[row] > capL[row]) {
                capL[row] <<= 1;
            }
            char *t = (char *) malloc(sizeof(char) * capL[row]);
            for (int i = 0; i < col; i++) {
                t[i] = str[row][i];
            }
            free(str[row]);
            str[row] = t;
        }
        for (int i = col; i < col + len; i++) {
            str[row][i] = src[i - col];
        }
        str[row][col + len] = '\n';
        str[row][col + len + 1] = 0;
        //处理 row + 1 至 row + cntl - 1 行，为 src 第 2 行至倒数第二行
        for (int i = row + 1, k = len + 1; i < row + cntl && k < lens; i++) {   //k:第i行对应src中的起始下标
            len = 0;    //第i行'\n'前的字符数
            while (src[k + len] != '\n' && k + len < lens) len++;
            sizeL[i] = len + 2;
            capL[i] = 1;
            while (capL[i] < sizeL[i]) {
                capL[i] <<= 1;
            }
            str[i] = (char *) malloc(sizeof(char) * capL[i]);
            for (int j = 0; j < len; j++) {
                str[i][j] = src[k + j];
            }
            str[i][len] = '\n';
            str[i][len + 1] = 0;
            k += len + 1;
        }
    } else {
        sizeL[row] += lens;
        if (sizeL[row] > capL[row]) {   //装不下了，重新开
            while (sizeL[row] > capL[row]) {
                capL[row] <<= 1;
            }
            char *t = (char *) malloc(sizeof(char) * capL[row]);
            for (int i = 0; i < col; i++) {
                t[i] = str[row][i];
            }
            for (int i = col; i < col + lens; i++) {
                t[i] = src[i - col];
            }
            for (int i = col + lens; i < sizeL[row]; i++) {
                t[i] = str[row][i - lens];
            }
            free(str[row]);
            str[row] = t;
        } else {
            for (int i = sizeL[row] - 1; i >= col + lens; i--) {
                str[row][i] =  str[row][i - lens];
            }
            for (int i = col; i < col + lens; i++) {
                str[row][i] = src[i - col];
            }
        }
    }
}

/**
 * 删除字符
 * pos:删除的位置（注意不是光标前的位置，而是光标所在的位置）
 * doRecord:是否需要记录操作  0-不需要  1-需要
 */ 
void deleteChar(RCNode pos, int doRecord) {
    int row = pos.row - 1, col = pos.column - 1;
    if (doRecord) {
        record(OP_DELETE, pos, CharToString(str[row][col]));
    }
    if (str[row][col] == '\n') {     //若删去的是换行符则行往上移
        //下一行拼接到这一行
        if (sizeL[row] + sizeL[row + 1] - 2 > capL[row]) {   //装不下了，重开
            while (sizeL[row] + sizeL[row + 1] - 2 > capL[row]) {
                capL[row] <<= 1;
            }
            char *t = (char *) malloc(sizeof(char) * capL[row]);
            for (int i = 0; i < sizeL[row] - 2; i++) {
                t[i] = str[row][i];
            }
            for (int i = 0; i < sizeL[row + 1]; i++) {
                t[sizeL[row] - 2 + i] = str[row + 1][i];
            }
            sizeL[row] += sizeL[row + 1] - 2;
            free(str[row]);
            str[row] = t;
        } else {
            for (int i = 0; i < sizeL[row + 1]; i++) {
                str[row][sizeL[row] - 2 + i] = str[row + 1][i];
            }
            sizeL[row] += sizeL[row + 1] - 2;
        }
        //行往上移
        free(str[row + 1]);
        sizeR--;
        for (int i = row + 1; i < sizeR; i++) {
            str[i] = str[i + 1];
            sizeL[i] = sizeL[i + 1];
            capL[i] = capL[i + 1];
        }
    } else {    //若删去普通字符，则这行的右边往左移
        sizeL[row]--;
        for (int i = col; i < sizeL[row]; i++) {
            str[row][i] = str[row][i + 1];
        }
    }
}

/**
 * 删除选中的字符串
 * start:起始位置
 * end:终止位置
 * doRecord:是否需要记录操作  0-不需要  1-需要
 * 注意删除的范围是[start, end)左闭右开区间
 */ 
void deleteSelectString(RCNode start, RCNode end, int doRecord) {
    if (start.row > end.row || (start.row == end.row && start.column > end.column)) {
        RCNode t = start;
        start = end;
        end = t;
    }
    if (start.row == end.row && start.column == end.column) return;
    if (doRecord) {
        string deleteStr = getSelectString(start, end);
        record(OP_DELETE, start, deleteStr);
    }
    if (start.row == end.row) {
        int row = start.row - 1, len = end.column - start.column;
        sizeL[row] -= len;
        for (int i = start.column - 1; i < sizeL[row]; i++) {
            str[row][i] = str[row][i + len];
        }
    } else {
        --start.row, --start.column, --end.row, --end.column;   //RCNode下标从1开始而二维数组从0开始
        //删除部分结尾行补到起始行
        sizeL[start.row] = start.column + sizeL[end.row] - end.column;
        if (sizeL[start.row] > capL[start.row]) {   //装不下了
            while (sizeL[start.row] > capL[start.row]) {
                capL[start.row] <<= 1;
            }
            char *t = (char *) malloc(sizeof(char) * capL[start.row]);
            for (int i = 0; i < start.column; i++) {
                t[i] = str[start.row][i];
            }
            for (int i = start.column; i < sizeL[start.row]; i++) {
                t[i] = str[end.row][end.column + i];
            }
            free(str[start.row]);
            str[start.row] = t;
        } else {
            for (int i = start.column; i < sizeL[start.row]; i++) {
                str[start.row][i] = str[end.row][end.column + i];
            }
        }
        //删除中间的行
        sizeR -= end.row - start.row;
        for (int i = start.row + 1; i <= end.row; i++) {
            free(str[i]);
        }
        //后面的行往上补
        for (int i = start.row + 1; i < sizeR; i++) {
            str[i] = str[i + end.row - start.row];
            sizeL[i] = sizeL[i + end.row - start.row];
            capL[i] = capL[i + end.row - start.row];
        }
    }
}