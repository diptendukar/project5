#include "stdio.h"

int
main(){
    const int base_addr =0xffff; 
    int b;
    scanf("%d", &b);
    int c = base_addr + b;
    return c;
}