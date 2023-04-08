## Introduction
`mem.c` contains a memory leak, i.e., depending on the value of your input, it allocate a piece of memory which is not freed at the end of the main function. 

Your job is to make sure all allocated memory are freed properly.

More specifically, you need to wrap `malloc` and `free`, let's call your wrapped `malloc` and `free` `w_malloc` and `w_free` instead. 

- Your `w_malloc` needs to wrap the real `malloc`. On top of it, it also needs to 1. Properly error handle when `malloc` call failed. 2. When `malloc` succeeds, you need to store the pointer returned by `malloc` in an array for bookkeeping purpose. The array can be statically reserved in a global area. 

- Accordingly, your `w_free` needs to properly release the bookkept pointer from the array.

- Lastly, at the end of the `main` function, as you can see there is a `whole_free` function call. In this function, you need to free all the pointers that are still in the array (i.e., not properly freed by calls to `w_free`). 

## Steps
- Implement your `w_malloc` `w_free` and `whole_free` in `wrapped_malloc.c` and `wrapped_malloc.h`
- type `make` to compile the executable.