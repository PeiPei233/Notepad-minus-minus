/*
    主要包括复制 剪切 粘贴功能
    这一部分应该要和Windows剪贴板进行交互，自行查找资料
*/

#include "global.h"

/*
    根据传入的起始行列坐标与终止行列坐标进行复制
*/
void copyText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
}

/*
    根据传入的起始位置粘贴文本内容（即需要修改对应的文件）
*/
void pasteText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
}

/*
    根据传入的起始行列坐标与终止行列坐标进行剪切
    即需要先复制这段内容，再将其删除
*/
void shearText() {
    RCNode startSelect = getSelectStartRC();
    RCNode endSelect = getSelectEndRC();
}