# UnableToCount

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