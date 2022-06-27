/*
    ���ļ���Ҫ�����򿪳���ʱ�ĳ�ʼ��ͼ�ν��棬�����˵����ı���ʾ�ȵȡ�
    �ǵ��޸Ķ�Ӧ��ͬ��ͷ�ļ����ټӵ�main��include��
*/

#include <stdio.h>
#include "global.h"
#include "display.h"
#include "libgraphics.h"
#include "imgui.h"
#include "style.h"
//��Ҫ��Ҫʵ�����º���

TextStyle textStyle;

/*
    ��ʼ�������Զ�����ɫ
*/
void initColor() {
    DefineColor("Button Gray", 243 * 1./255, 243 * 1./255, 243 * 1./255);
    DefineColor("Button Hot Gray", 225 * 1./255, 225 * 1./255, 225 * 1./255);
    DefineColor("Menu Gray", 221 * 1./255, 221 * 1./255, 221 * 1./255);
    DefineColor("Menu Hot Gray", 198 * 1./255, 198 * 1./255, 198 * 1./255);
    DefineColor("Textbox Hot Blue", 0, 144 * 1./255, 241 * 1./255);
    DefineColor("Select Blue", 173 * 1./255, 214 * 1./255, 255 * 1./255);
    DefineColor("Table Gray", 249 * 1./255, 249 * 1./255, 249 * 1./255);
    DefineColor("Link Blue", 0, 62 * 1./255, 146 * 1./255);
    DefineColor("Link Hot Blue", 0, 26 * 1./255, 104 * 1./255);
}

/**
 * ��ȡ��ǰ����������ʽ
 */ 
TextStyle getTextStyle() {
    return textStyle;
}

/**
 * ���õ�ǰ����������ʽ�����浽�����ļ���
 */ 
void setTextStyle(TextStyle newTextStyle) {
    textStyle = newTextStyle;
    updateTotalDisplayRow();
    FILE *styleFile = fopen("./styleConfig.properties", "w");
    fprintf(styleFile, "fontFamily=%s\nfontSize=%d\nlineSpacing=%.16lf\ntextColor=%s\nbackgroundColor=%s", textStyle.fontFamily, textStyle.fontSize, textStyle.lineSpacing, textStyle.textColor, textStyle.backgroundColor);
    fclose(styleFile);
}

/**
 * ��ʮ��������ɫ�붨������ɫ
 */
int defineColorRGB(char *des, char *rgb) {
    if (strlen(rgb) != 7) return 0;
    if (rgb[0] != '#') return 0;

    string s = CopyString(rgb);
    
    for (int i = 1; i < strlen(rgb); i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] = s[i] - 'a' + 'A';
        }
        if ((s[i] < '0' || s[i] > '9') && (s[i] < 'A' || s[i] > 'F')) {
            free(s);
            return 0;
        }
    }

    int r;
    if (s[1] >= 'A') r = s[1] - 'A' + 10;
    else r = s[1] - '0';
    if (s[2] >= 'A') r = r * 16 + s[2] - 'A' + 10;
    else r = r * 16 + s[2] - '0';

    int g;
    if (s[3] >= 'A') g = s[3] - 'A' + 10;
    else g = s[3] - '0';
    if (s[4] >= 'A') g = g * 16 + s[4] - 'A' + 10;
    else g = g * 16 + s[4] - '0';

    int b;
    if (s[5] >= 'A') b = s[5] - 'A' + 10;
    else b = s[5] - '0';
    if (s[6] >= 'A') b = b * 16 + s[6] - 'A' + 10;
    else b = b * 16 + s[6] - '0';

    // printf("%s:%d %d %d\n", s, r, g, b);

    DefineColor(des, r * 1./255, g * 1./255, b * 1./255);
    
    free(s);

    return 1;
} 

static char t[1000010];

/*
    ��ʼ��չʾ����
*/
void initDisplay() {

    //��ȡ�û���������ļ�
    FILE *styleFile;
    if (!(styleFile = fopen("./styleConfig.properties", "r"))) {
        textStyle.fontFamily = CopyString("΢���ź�");
        textStyle.fontSize = 15;
        textStyle.lineSpacing = 1.2;
        textStyle.textColor = CopyString("#000000");
        textStyle.backgroundColor = CopyString("#FFFFFF");
        setTextStyle(textStyle);
    } else {
        textStyle.fontFamily = CopyString("΢���ź�");
        textStyle.fontSize = 15;
        textStyle.lineSpacing = 1.2;
        textStyle.textColor = CopyString("#000000");
        textStyle.backgroundColor = CopyString("#FFFFFF");
        for (int _i = 1; _i <= 5; _i++) {
            fscanf(styleFile, " %[a-zA-Z] ", t);
            fscanf(styleFile, " = ");
            if (StringEqual(t, "fontFamily")) {
                t[0] = t[1] = 0;
                fscanf(styleFile, " %[^\n]", t);
                for (int i = strlen(t) - 1; i >= 0 && t[i] == ' '; i--) {
                    if (t[i] == ' ') t[i] = 0;
                }
                if (strlen(t)) textStyle.fontFamily = CopyString(t);
            } else if(StringEqual(t, "fontSize")) {
                int size;
                fscanf(styleFile, "%d", &size);
                textStyle.fontSize = size > 0 ? size : 15;
            } else if(StringEqual(t, "lineSpacing")) {
                double ls;
                fscanf(styleFile, "%lf", &ls);
                textStyle.lineSpacing = ls > 0 ? ls : 1.2;
            } else if(StringEqual(t, "textColor")) {
                fscanf(styleFile, "%s", t);
                if (defineColorRGB("tmpTextColor", t)) {
                    textStyle.textColor = CopyString(t);
                }
            } else if(StringEqual(t, "backgroundColor")) {
                fscanf(styleFile, "%s", t);
                if (defineColorRGB("tmpBackgroundColor", t)) {
                    textStyle.backgroundColor = CopyString(t);
                }
            }
        }
        fclose(styleFile);
        // printf("STYLE:%s %d %lf %s %s\n", textStyle.fontFamily, textStyle.fontSize, textStyle.lineSpacing, textStyle.textColor, textStyle.backgroundColor);
    }

    initColor();
    updateTotalDisplayRow();
    setButtonColors("Button Gray", "Black", "Light Gray", "Black", 1);
    setTextBoxColors("White", "Black", "Textbox Hot Blue", "Black", 0);
    setMenuColors("Menu Gray", "Black", "Menu Hot Gray", "Black", 1);
    display();
}