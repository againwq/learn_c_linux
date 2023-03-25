#include<stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y)) //带参数的宏定义
#define PI 3.14
#define V PI * 4 / 3 * 10 * 10  //嵌套定义

int main(int argc, char const *argv[])
{
    int r;
    printf("请输入圆的半径");
    scanf("%d", &r);

    float s = PI * r * r;
    printf("半径为10的球的体积：%f\n", V);

#undef PI  //结束宏定义
    printf("10和20谁更大：%d\n", MAX(10, 20));
    printf("面积：%f", s);
    return 0;
}
