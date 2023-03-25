//指针数组    数组里面每个元素都是指针
#include<stdio.h>

int main(int argc, char const *argv[])
{
    char *strs[] ={
        "wadawdg",
        "eeasd",
        "eaffeea",
        "wqsadaf"
    };
    for(int i = 0; i < 4; i++){
        printf("%s\n", strs[i]);
    }
}
