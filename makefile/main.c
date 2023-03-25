#include <stdio.h>
#include "add/add.h"
#include "sub/sub.h"

int main(int argc, char const *argv[])
{
    int a = 10, b = 30;
    float c = 2.56, d = 3.98;
    printf("%d\n", sub_int(a, b));
    printf("%.2f", add_float(c, d));
    return 0;
}
