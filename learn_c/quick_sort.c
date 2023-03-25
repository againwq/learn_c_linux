#include<stdio.h>

void exchange_value(int arr[], int a, int b){
    int temp = arr[a];
    arr[a] = arr[b];
    arr[b] = temp;
}

void sort(int arr[], int left, int right){

    int middle = (left + right) / 2, i = left, j = right;

    while (i <= j)
    {
        for(; arr[i] < arr[middle]; i ++){}
        for(; arr[j] > arr[middle]; j --){}
        if(i <= j){
            exchange_value(arr, i, j);
            i++;
            j--;
        }
    }

    if(left < j){
        sort(arr, left, j);
    }
    if(i < right){
        sort(arr, i, right);
    }
}

int main(int argc, char const *argv[])
{
    int i, length;
    int arr[] = {15,23,38,75,42,96,13,17,93,85,45,65,69,41};

    length = sizeof(arr) / sizeof(arr[0]);

    sort(arr, 0, length - 1);
    for(i = 0; i < length; i++){
        printf("%d\n", arr[i]);
    }
    return 0;
}
