#include <stdio.h>

int main(int argc, char const *argv[])
{
    int n;
    printf("请输入数组长度:");
    scanf("%d", &n);
    char testArr[n];

    for(int i = 0; i < n; i++){
        scanf("%c", &testArr[i]);
    }

    for(int i = 0; i < n; i++){
        printf("%c\n", testArr[i]);
    }
}
