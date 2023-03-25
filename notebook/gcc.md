# gcc 基本使用

## gcc编译过程

    预编译 -> 编译和优化 -> 翻译成汇编 -> 链接

### 详细解释

- 所谓预编译，就是把原文件中的宏定义进行替换，并把头文件声明的文件包含进源代码中
- 将源代码翻译成操作码，通常翻译成汇编
- 解析引用文件之外的的函数和变量，将其他已经编写好的目标文件引用过来

### 相关命令

- `gcc -o test main.c` : 将main.c编译成可执行文件test(一步到位)

- `gcc -c main.c` : 将main.c编译成目标文件main.o

- `gcc -o test string.o main.o` : 将string.o和main.o进行链接，生成test可执行文件

- `gcc -o string.i -E string.c` : 对string.c进行预编译生成string.i（string.i的内容是头文件和宏定义替换之后的内容）

- `gcc -S string.c` : string.c编译成汇编文件string.s

----------------------------------

## 生成动态链接库和静态链接库

### 静态链接库的生成和使用

</br>
静态链接库常常以 .a 为后缀,通常使用 **ar** 命令生成</p>
例如 `ar -rcs libstr.a string.o` 就是将string.o打包为库文件libstr.a，可以通过</p>
`gcc -o test main.c libstr.a` 来将libstr.a链接到main.c上生成可执行文件文件</p>
</br>

### 动态链接库的生成和使用
