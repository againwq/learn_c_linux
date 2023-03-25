#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    char str[100] = "eadsfawdwa";
    char *p = &str[0];
    printf("%c\n", *(p+1));

    char *str1 = "wwd dwaert 8w6a8dw";
    int i;
    for(i = 0; i < strlen(str1) ; i++){
        printf("%c ", str1[i]);// *(str + i) 或者str[i]都可以访问
    }
    
}
