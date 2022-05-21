/**
 * 
 * ���ļ����ڴ����ַ�������Ĵ������
 * 
 */ 

#include "storage.h"
#include "unredo.h"

unsigned int sizeR;      //�з�����������
unsigned int capR;       //�з���ɳ�������
unsigned int *sizeL;     //�з�����������
unsigned int *capL;      //�з���ɳ�������
char **str;

/**
 * ��ʼ���洢
 * ��֮ǰ�����ݻᱻ�����֮ǰ�����ݻᶪʧ��
 */
void initStorage() {
    if (sizeR) {    //���֮ǰ������
        for (int i = 0; i < sizeR; i++) {
            free(str[i]);
        }
        free(str);
        free(sizeL);
        free(capL);
    }
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
 * ���ĳһ�е��ַ�������Ҫ�޸ģ���Ҫ�޸ģ���Ҫ�޸ģ������ַ���������ʹ�ã���
 * �����Ҫ���޸ĵ�ĳһ�е��ַ�����ʹ��getContent()��������
 */ 
string getRowContent(int row) {
    if (row <= sizeR && row >= 1) return str[row - 1];
    else return NULL;
}

/**
 * ���������
 * ������ĩ����
 */ 
unsigned int getTotalRow() {
    return sizeR;
}

/**
 * ���ѡ�з�Χ���ַ����ĳ���
 * �������е�'\n'
 * ע���õ��ַ������ȷ�ΧΪ[start, end)����ҿ�����
 */
unsigned int getContentLength(RCNode start, RCNode end) {
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
 * ���ѡ�з�Χ���ַ�����Ϊ�½��ַ��������޸ģ�
 * ����ǵü�ʱfree
 * ע���õ��ַ�����ΧΪ[start, end)����ҿ�����
 * ���ַ����򷵻�NULL
 */ 
string getContent(RCNode start, RCNode end) {
    if (start.row > end.row || (start.row == end.row && start.column > end.column)) {
        RCNode t = start;
        start = end;
        end = t;
    }
    unsigned int len = getContentLength(start, end);
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
 * ���ĳһ�еĳ���
 * �����ĩ��'\n'�򳤶��а���'\n'
 */ 
unsigned int getRowLength(int row) {
    if (row <= sizeR && row >= 1) return sizeL[row - 1] - 1;     //ÿһ�еĴ�С��������'\0' Ҫ����
    else return 0;
}

/**
 * ����ַ�
 * pos:��ӵ�λ��
 * ch:��ӵ��ַ�
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addContentByChar(RCNode pos, char ch, int doRecord) {
    int row = pos.row - 1, col = pos.column - 1;    //���������±���㿪ʼ
    if (doRecord) {
        string s = (char *) malloc(sizeof(char) * 2);
        s[0] = ch;
        s[1] = 0;
        record(OP_ADD, pos, s);
    }
    if (ch == '\n') {   //���ǻ��з������¿�һ�У�����������
        sizeR++;
        if (sizeR > capR) { //�з���װ�����ˣ��ؿ�
            capR <<= 1;
            char **tmpStr = (char **) malloc(sizeof(char *) * capR);
            unsigned int *tmpSizeL = (int *) malloc(sizeof(unsigned int) * capR);
            unsigned int *tmpCapL = (int *) malloc(sizeof(unsigned int) * capR);
            //���˲��������������������
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
    } else {    //��ͨ�ַ����λ��֮���������һ��
        sizeL[row]++;
        if (sizeL[row] > capL[row]) {   //װ�����ˣ��ؿ�
            capL[row] <<= 1;
            char *t = (char *) malloc(sizeof(char) * capL[row]);
            //��������
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
 * ����ַ���
 * start:��ʼλ��
 * src:��ӵ��ַ���
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addContentByString(RCNode start, string src, int doRecord) {
    unsigned int lens = strlen(src);
    if (!lens || !src) return;
    if (doRecord) {
        string addStr = (char *) malloc(sizeof(char) * (lens + 1));
        for (int i = 0; i <= lens; i++) {
            addStr[i] = src[i];
        }
        record(OP_ADD, start, addStr);
    }

    int row = start.row - 1, col = start.column - 1;

    //���㻻�з��ĸ���
    unsigned int cntl = 0;
    for (int i = 0; i < lens; i++) {
        if (src[i] == '\n') cntl++;
    }
    if (cntl) {     //�л���
        sizeR += cntl;  //ÿ�����з���Ҫ���µ�һ��
        if (sizeR > capR) {     //װ�����ˣ����¿�
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
        //����� row + cntl �У�Ϊ src ���һ����� row �к��ε�ƴ��
        unsigned int len = 0;
        while (src[lens - len - 1] != '\n' && len < lens) len++;    //���һ�еĳ���
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
        //����� row �У�Ϊ�� row ��ǰ����� src ��һ�е�ƴ��
        len = 0;
        while (src[len] != '\n' && len < lens) len++;   //src ��һ�е�'\n'ǰ�ĳ���
        sizeL[row] = col + len + 2;
        if (sizeL[row] > capL[row]) {   //װ�����ˣ��ؿ���������Ҫ������������
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
        //���� row + 1 �� row + cntl - 1 �У�Ϊ src �� 2 ���������ڶ���
        for (int i = row + 1, k = len + 1; i < row + cntl && k < lens; i++) {   //k:��i�ж�Ӧsrc�е���ʼ�±�
            len = 0;    //��i��'\n'ǰ���ַ���
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
        if (sizeL[row] > capL[row]) {   //װ�����ˣ����¿�
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
 * ������ݣ����鷳��
 * by:��ӷ�ʽ BY_CHAR ͨ���ַ���ʽ��� BY_STRING ͨ���ַ�����ʽ���
 * pos:���λ�ã���ʼλ�ã�
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 */ 
void addContent(int by, RCNode pos, char *src, int doRecord) {
    if (by == BY_CHAR) {
        addContentByChar(pos, *src, doRecord);
    } else if (by == BY_STRING) {
        addContentByString(pos, src, doRecord);
    }
}

/**
 * ɾ������
 * start:��ʼλ��
 * end:��ֹλ��
 * doRecord:�Ƿ���Ҫ��¼����  0-����Ҫ  1-��Ҫ
 * ע��ɾ���ķ�Χ��[start, end)����ҿ�����
 */ 
void deleteContent(RCNode start, RCNode end, int doRecord) {
    if (start.row > end.row || (start.row == end.row && start.column > end.column)) {
        RCNode t = start;
        start = end;
        end = t;
    }
    if (start.row == end.row && start.column == end.column) return;
    if (doRecord) {
        string deleteStr = getContent(start, end);
        record(OP_DELETE, start, deleteStr);
    }
    if (start.row == end.row) {
        int row = start.row - 1, len = end.column - start.column;
        sizeL[row] -= len;
        for (int i = start.column - 1; i < sizeL[row]; i++) {
            str[row][i] = str[row][i + len];
        }
    } else {
        --start.row, --start.column, --end.row, --end.column;   //RCNode�±��1��ʼ����ά�����0��ʼ
        //ɾ�����ֽ�β�в�����ʼ��
        sizeL[start.row] = start.column + sizeL[end.row] - end.column;
        if (sizeL[start.row] > capL[start.row]) {   //װ������
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
        //ɾ���м����
        sizeR -= end.row - start.row;
        for (int i = start.row + 1; i <= end.row; i++) {
            free(str[i]);
        }
        //����������ϲ�
        for (int i = start.row + 1; i < sizeR; i++) {
            str[i] = str[i + end.row - start.row];
            sizeL[i] = sizeL[i + end.row - start.row];
            capL[i] = capL[i + end.row - start.row];
        }
    }
}