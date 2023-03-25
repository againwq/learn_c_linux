# 0 "string.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "string.c"


int StrLen(char *str){
    int len = 0;
    while (*str++ != '\0')
    {
        len++;
    }
    return len;
}
