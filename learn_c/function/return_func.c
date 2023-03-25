#include <stdio.h>

float add(float a, float b)
{
    return a + b;
}
float sub(float a, float b)
{
    return a - b;
}

float cheng(float a, float b)
{
    return a * b;
}

float chu(float a, float b)
{
    return a / b;
}
// 将函数指针作为返回值
float (*select(char symbol))(float, float)
{
    switch (symbol)
    {
    case '+':
        return add;
        break;
    case '-':
        return sub;
        break;
    case '*':
        return cheng;
        break;
    case '/':
        return chu;
        break;
    default:
        break;
    }
}

int main(int argc, char const *argv[])
{
    char symbol;
    float num1, num2;

    float (*fp)(float, float) = NULL;

    float res;

    scanf("%f%c%f", &num1, &symbol, &num2);

    printf("%d\n", getchar());

    fp = select(symbol); //接收返回的函数指针

    res = (*fp)(num1, num2); //使用函数指针调用函数

    printf("%f", res);
    return 0;
}
