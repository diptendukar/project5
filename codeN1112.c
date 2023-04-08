// Fix the unintialized variable using appropriate technique that fits according to the rest of the code
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

	// initialize variable 
    int *px = (int *) malloc(sizeof(int));
    float foo;

    if (px) {
        foo = 3.5;
        *px = argc - 1;
        if (*px == 1) {
            printf("%6.1f", foo);
        } else {
			// add check for div by 0
			
			if (*px != 0) {
                printf("%6.1f", 100.00 / *px);
            } else {
                printf("Division by zero is not allowed. Add aruguments\n");
            }
         
            //free(px); removed this as outer free handles this - double free
        }
        free(px);
    }
    return 0;
}
