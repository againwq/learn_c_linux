#include <stdio.h>

void twoSum(int* nums, int numsSize, int target, int *returnSize){
    int i, j;
    for(i = 0; i < numsSize; i++){
        for(j = i + 1; j < numsSize; j++){
            if(nums[i] + nums[j] == target){
                returnSize[0] = i;
                returnSize[1] = j;
                return;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    int *arr = NULL;
    int arrTest[4] = {2, 7, 11, 15};
    twoSum(arrTest, 4, 9, arr);

    for(int i = 0; i < 2; i++){
        printf("%d\n", arr[i]);
    }
    
    return 0;
}

