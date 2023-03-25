#include<stdio.h>

enum Week {
    sun=7, mon=1, tue=2, wed=3, thu=4, fri=5, sat=6      //默认0，1，2，3，.....
};
//sun,mon, tue这些枚举常量是不能改变的
int main(int argc, char const *argv[])
{
    enum Week today;  
    
    for ( today = mon; today < sun; today ++) 
    {
        printf("%d\n", today);
    }
    
    return 0;
}
