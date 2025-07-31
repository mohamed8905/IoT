#include <stdio.h>
typedef struct {
    int first;
    int second;
} Pair;

int lowerBound(int nums[], int numsSize, int target);
int upperBound(int nums[], int numsSize, int target);
Pair searchRange(int nums[], int numsSize, int target);


int main() {
    int nums[] = {5, 7, 7, 8, 8, 8, 9};
    int numsSize = 7;
    int target = 8;
    
    Pair result = searchRange(nums, numsSize, target);

    printf("Output: [%d, %d]\n", result.first, result.second);
    
    return 0;
}
int lowerBound(int nums[], int numsSize, int target) {
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
    
    return (nums[left] == target) ? left : -1;
}
int upperBound(int nums[], int numsSize, int target) {
    int left = 0;
    int right = numsSize;
    
    while (left < right) {
        int mid = left + (right - left) / 2;
        
        if (nums[mid] <= target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    
    return (nums[right-1] == target) ? right-1 : -1;
}
Pair searchRange(int nums[], int numsSize, int target) {
    Pair result;
    
    if (numsSize == 0) {
        result.first = -1;
        result.second = -1;
        return result;
    }
    
    result.first = lowerBound(nums, numsSize, target);
    result.second = upperBound(nums, numsSize, target);
    
    return result;
}