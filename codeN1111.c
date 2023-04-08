#include <stdio.h>
#include <stdlib.h>

#define MAX_LEN 10

void buildMatrix(unsigned int matrix_size) {
    char **square = calloc(matrix_size, sizeof(char *));
    for (int i = 0; i < matrix_size; ++i) {
        square[i] = calloc(matrix_size, sizeof(char));
    }
    system("cls");
    printf("\tMatrix\n");

    for (int i = 0; i < matrix_size; i++) {
        printf("\n");
        for (int j = 0; j < matrix_size; j++) {
            square[i][j] = i + j;
            printf("  %d  |", i + j);
        }
    }
    
    // added code for free 
	for (int i = 0; i < matrix_size; ++i) {
	free(square[i]);
    }
    free(square);
    
}

int main() {
    int matrix_size = 3;
    printf("Enter size of n x n matrix");
    
    // additional check for scanf cos you can input alphabets
    
    
    int ret = scanf("%d", &matrix_size);

    if (ret != 1) {
        printf("Invalid input!\n");
        return 1;
    }
    
    
    
    if (matrix_size > MAX_LEN) {
        printf("Exceeded max length!");
    }
    
    // if within max len then build matrix 
    else {
    buildMatrix(matrix_size);
	}
    return 0;

}
