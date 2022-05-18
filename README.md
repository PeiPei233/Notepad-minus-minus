# UnableToCount


## Stage 2

**TODO: undo/redo 二维数组**

**DDL：5.25晚上**

**DDL：5.25晚上**

**DDL：5.25晚上**

### 二维数组

```c
//storage.c
int sizeR = 0;   //目前占用的大小
int capR = 1;   //目前开的总空间大小
int *sizeL;
int *capL;
char **str = (char **) malloc(sizeof(char *) * capR);     //指向某一行

str[0] = (char *) malloc(sizeof(char) * ...);

//遇到换行
sizeR++; capR? malloc?

//addChar
//行内：
sizeL[i] ? capL[i] : malloc or not
//换行符

//deleteChar 


string getRowString() {return str[row];}
int getRowTotal() {return sizeR};
string getSelectString(RCNode startSelect, RCNode endSelect);
int getRowLen(int row);
void addChar(RCNode loc);
void addString(RCNode start, string src);
void deleteChar(RCNode loc);
void deleteSelectString(RCNode start, RCNode endSelect);


```

### redo/undo
```c
//unredo.c
enum {
    DO_ADD;
    DO_DELETE;
};

typedef struct node {
    int op;
    RCnode loc;
    string str;
    struct node *next;
    struct node *last;
} *linkedList;

void record(int op, RCNode loc, string str) {
    //根据目前的光标位置，选择范围来操作
}
void undo();
void redo();
```

### 分锅：
edit: zhb

content: yps

修改:zj


## 第一阶段

**DDL 5.1晚上**

**DDL 5.1晚上**

**DDL 5.1晚上**

文件中给出的函数参数可以自行调整，给出的函数参数主要是先让其他人能够调用这个函数做事情。

关于代码规范：

 - 请尽可能使用大/小驼峰法命名，如你已经看到过的 `drawSelectRange()`。当然一些显而易见的命名也不是不可以，正如你已经看到过的 `src`、`tar` 分别代表来源 (Source) 以及目标 (Target)。
 
 - 写一些必要的注释，例如解释某个莫名其妙的变量的作用，防止未来只有编译器能看懂自己的代码，也方便写不出来的时候帮忙调试。

 - 自行添加的新函数请在函数前添加注释，包括但不仅限于函数作用、用法以及返回值。并且记得更新头文件！示例如下：
 ```c
 /**
  * 该函数用来设置当前的光标位置
  * 使用方法：setCursorRC(newCursor);
  * 传入参数：newCursor 新的光标位置
  * 返回值：无
  */
 void setCursorRC(RCNode newCursor); 
 ```

 - 具体代码规范不做要求，只要简洁就好哩

关于 VSCode 中 Makefile 的使用：

 - 由于已经配置好 Makefile 以及 task.json，在 Vscode 中按 `Ctrl+Shift+B` 即可自行编译，若有报错会显示。编译后在下方终端中（或者 `` Ctrl+` ``）输入 `./main` 即可执行程序。（要先配好 MinGW 的环境变量！）

 - 在 push 到 GitHub 仓库前请先在终端中执行 `mingw32-make clean` 删除`*.o`、`*.d`、`main.exe` 文件后再 push



zhb:
maxn为图形界面每行最多的字符数

currentFile.txt为当前储存输入内容的文件名

