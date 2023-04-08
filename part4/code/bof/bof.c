#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef SRC_LEN
#define SRC_LEN 10
#endif

int foo(){
    char dest_str[10] = "ImDestStr";
    int i = 0;
    char src_buf[SRC_LEN];
    for(i = 0 ; i < SRC_LEN ; i ++){
        src_buf[i] = 'a';
    }
    src_buf[SRC_LEN - 1] = '\0';
    strcpy(dest_str, src_buf);
    return 0;
}

int main(int argc, char** argv) {
    
    foo();
    return 0;
}