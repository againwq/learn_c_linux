#include<stdio.h>
#include<stdlib.h>

int main(int argc, char const *argv[])
{
    int *arr = NULL;
    int count = 0;
    int temp;
    while(temp != -1){
        scanf("%d", &temp);
        getchar();
        count ++;
        arr = realloc(arr, sizeof(int) * count); //重新分配内存
        arr[count - 1] = temp;
    }
    for (int i = 0; i < count - 1; i++)
    {
        printf("%d\n", arr[i]);
    }
    
    return 0;
}
