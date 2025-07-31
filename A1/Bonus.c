#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dfs(char board[3][4], int visited[3][4], int i, int j, char* word, int wordIndex, int rows, int cols) {
    if (wordIndex == strlen(word)) return 1; 
    if (i < 0 || i >= rows || j < 0 || j >= cols) return 0;
    if (visited[i][j] || board[i][j] != word[wordIndex]) return 0;  // Already visited or wrong char
    
    visited[i][j] = 1;
    
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}}; 
    
    for (int d = 0; d < 4; d++) {
        int newI = i + directions[d][0];
        int newJ = j + directions[d][1];
        
        if (dfs(board, visited, newI, newJ, word, wordIndex + 1, rows, cols)) {
            return 1;
        }
    }
    
    visited[i][j] = 0;
    return 0;
}
int exist(char board[3][4], char* word, int rows, int cols) {
    int visited[3][4] = {{0}};
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (dfs(board, visited, i, j, word, 0, rows, cols)) {
                return 1;
            }
        }
    }
    return 0;
}
int main() {
    char board[3][4] = {{'A','B','C','E'},{'S','F','C','S'},{'A','D','E','E'}};     
    char target[] = "ABCCED";    

    int result = exist(board, target, 3, 4);
    printf("Word exists: %s\n", result ? "true" : "false");
    
    return 0;
}