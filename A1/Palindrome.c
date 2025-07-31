#include <stdio.h>
#include <string.h>

int main() {
    char text[100];
    printf("Type a word: ");
    scanf("%s", text);

    int len = strlen(text);
    for (int i = 0; i < len/2; i++) {
        if (text[i] != text[len-1-i]) {printf("The word is not a palindrome"); return 0;}
    }
    printf("The word is a palindrome");
    
    return 0;
}