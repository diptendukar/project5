#include "stdio.h"
#include "stdlib.h"
#include "wrapped_malloc.h"

int
main(){
    void * a = w_malloc(4);
    void * b;
    int choice;
    scanf("%d", &choice);
    if(choice == 0){
        b = w_malloc(5);
    }
    w_free(a);
    whole_free();
    return 0;
}