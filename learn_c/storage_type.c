/**
 * 链接属性： 
 * external（外部）：属于外部链接属性的标识符不论声明多少次，位于几个源文件都表示同一个实体。
 * internal（内部）：属于内部链接属性的标识符在同一个源文件内的所有声明中都指同一个实体，但位于不同的源文件中的多个声明属于不同的实体。 
 * none：没有链接属性的标识符总被当做单独的个体，表明该标识符的多个声明当做独立的不同的实体。
*/

/* 存储类型：
    auto: 默认存储类型，有空链接属性、自动存储期
    register： 和auto差不多，但是register类型数据的地址不允许被访问
    static： 有静态存储期，即便定义在局部，也是在程序结束后才释放。会将链接属性external变为internal。
    extern：定义external链接属性
    typedef 
*/
#include <stdio.h>

extern void func(); //声明func是外部的函数

int count = 520;

void test_static(){
    static int a = 10; //a仍是局部变量，但是函数调用完毕后a并不会被释放掉，程序结束后才会被释放
    printf("%d\n", a++);
}

void test_register(){
    register int a = 10; 
    //printf("%p", &a); // 对register类型的数据无法进行取址操作
}

int main(int argc, char const *argv[])
{
    for(int i = 0; i < 5; i++){
        test_static();
    }
    func(); //调用外部定义的函数
    return 0;
}
