#include <stdio.h>

extern int StrLen(char *str);

int main(int argc, char const *argv[])
{
    char str[] = "aaewdsa waer !wa";
    int len = StrLen(str);
    printf("%d", len);
    return 0;
}
