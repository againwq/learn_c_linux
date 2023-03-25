#include<stdio.h>


struct Person
{
    char name[10];
    int age;
    char sex;
};


int main(int argc, char const *argv[])
{
    struct Person people[3] = {     //结构体数组
        { "xqc", 20, '0'}, {"wsf", 18, '1'}, {"eea", 23, '1'}
    };
    printf("%s", people[0].name);
    return 0;
}