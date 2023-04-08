## Introduction
There is a `strcpy` function that is prone to buffer overflow in this `bof.c` file.
Depending on the value of `SRC_LEN`, if it is greater than the dest_str's length, then it will overflow dest_str's reserved space on the stack which is 10 bytes. This can have catastrophic consequence.

Your job is to write a LLVM pass to instrument this code **RIGHT BEFORE** it execute the potentially dangerous `strcpy` function, to check if an overflow would happen or not. 

- If the `src_buf` is longer than `dest_str`, then an overflow would definitely happen, you need to print properly and call `exit` to terminate the program. 
- If not, then you can print accordingly and go on executing this `strcpy`. 

There are a few assumptions to make your life easier.
1. Both `dest_str` and `src_buf` are properly '\0' terminated, so to check their length, you only need to call `strlen` on them.  Just imagine, if this was a `memcpy`, it will be much harder. 
2. The starter code of this magnificent buffer overflow sanitizer is under ```pass/bofsan```.


## Steps
- Modify Makefile here and CMakeLists.txt under bofsan, to point to your unzipped llvm-12 toolchain
- After finishing the bofsan.cpp, create a `build` folder alongside the `CMakeList.txt` file.
- `cd build`
- `cmake ..`
- `make`
    - check if this `LLVM pass` is compiled properly
- cd back here to this folder.
- modify line 3 of Makefile to point to the `bin` folder of `llvm-clang-12.0.0` toolchain that you downloaded and unzipped. 
- `SRC_LEN=15 make instrumented_bof`
- execute instrumented_bof by `./instrumented_bof` to see if the buffer overflow is intercepted or not. 

## the APIs you will be using:

- CreateCall:
    1. https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a2697fb3ba010f79193b9942081a2cbb7
    2. pass the function (which are already in the Pass Class), and pass in the arguments as arrays, returns a CallInst*

- CreateICmpSGT:
    1. https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a3280551f1232a760cb7b21e85fdd5eca
    2. Check is LHS is "Sign Greater Than" the RHS, return a bool value