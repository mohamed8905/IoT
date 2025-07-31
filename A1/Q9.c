#include <stdio.h>

int find(int nums[], int numsSize, int target);

int main() {
    int nums[] = {3, 5, 7, 8, 9, 12};
    int numsSize = 6;
    int target = 15;
    
    int result = find(nums, numsSize, target);

    printf("Answer is: %d", result);
    
    return 0;
}
int find(int nums[], int numsSize, int target) {
    int left = 0;
    int right = numsSize;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return left;
}