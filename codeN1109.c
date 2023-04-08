// Use C-Lang Static Analyzer to analyze and fix the bugs
// Hint this code has bugs that can be identified using static analyzer
// Also review the output generated to find any bugs/vulnerabilities that the static analyzer misses.

//MACROS 
#include<stdio.h>
// include stdlib for malloc
#include<stdlib.h>


//another custom array printing an array
void array_print(int* ptr3, int n)
{
	// initialize sub can be removed tbh
    int sub=0;
    
    // added null pointer check
    if (ptr3 == NULL) {
        printf("Error: Pointer is NULL.\n");
        return;
    }

    // passing n but not using n , this loop runs for 100000000 which is not defined by size_array passed as n, changing to n
    for(int i=0; i<n; i++)
    {
        ptr3[sub] = i;
        sub=sub+1;
        printf("Array is:%d\n ",ptr3[i]);
    }


}

int main(){
    // variable initializations 
    int* ptr_to_arr;
    int size_array=10;

    //Note for developers(students): use variables in method using defined pointers (not directly)



    ptr_to_arr = (int*)malloc(sizeof(int));
    // Hint:: add and assign the references to the pointers here:


    //function call statements
    array_print(ptr_to_arr,size_array);
    
    // added free for malloc
    free(ptr_to_arr);
    return 0;


}

