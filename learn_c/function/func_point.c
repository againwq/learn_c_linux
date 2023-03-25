#include<stdio.h>
//指针函数，返回一个指针,最好不要返回一个局部变量的地址，用常量地址就没问题
char *get_world(char c){
    switch (c)
    {
    case 'A': return "apple";
    case 'B': return "banana";
    case 'C': return "cap"; 
    default: return "end";
        break;
    }
}

int square(int num){
    return num * num;
}

int test_callback(int (*fp)(int)){
    return (*fp)(10);
}

int main(int argc, char const *argv[])
{
    char input;
    
    int (*fp)(int) = NULL; //函数指针，可以指向函数的指针

    fp = square;

    printf("%d\n", (*fp)(30));  //通过函数指针调用函数，如果函数指针作为参数，可以做到回调函数的效果

    printf("%d\n", test_callback(fp)); //回调函数

    scanf("%c", &input);
    getchar();  // 去除输入缓存里的最后一个字符，达到删除换行符的作用
    printf("%s\n", get_world(input));
    
    return 0;
}
