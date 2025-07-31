#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* addBinary(char* a, char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    int max_len = (len_a > len_b) ? len_a : len_b;
    
    char* result = malloc(max_len + 2);
    result[max_len + 1] = '\0';
    
    int carry = 0;
    int i = len_a - 1;
    int j = len_b - 1;
    int k = max_len;
    
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        
        if (i >= 0) sum += a[i] - '0';
        if (j >= 0) sum += b[j] - '0';
        
        result[k] = (sum % 2) + '0';
        carry = sum / 2;
        
        i--; j--; k--;
    }

    
    return result;
}
int main() {
    char a[10001];
    char b[10001];
    printf("Enter binary number a: ");
    scanf("%s", a);
    printf("Enter binary number b: ");
    scanf("%s", b);
    
    char* result = addBinary(a, b);
    
    printf("Output: %s\n", result);
    
    free(result);
    return 0;
}