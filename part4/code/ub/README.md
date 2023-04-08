## Introduction

Integer overflow is seemingly less dangerous compared to other serious vulnerabilities.
But when the overflown value is used at sensitive places (e.g., malloc), it can also have catastrophic results. 
In this part, we will help the developer to create a program whose integer could never overflow. 

## Experiment Description

In this experiment, you need to use llvm to iterate through all addition operations. 
- Promote each operand from 32 bit integer to 64 bit.
- Add those promoted operands together and check against the maximum value of a signed integer. 
- If the result is greater than the maximum value, print error message and exit.
- If not, print accordingly and go on executing the addition operation. 

## Steps
- Modify Makefile here and CMakeLists.txt under ubsan, to point to your unzipped llvm-12 toolchain
- After finishing the ubsan.cpp, create a `build` folder alongside the `CMakeList.txt` file.
- `cd build`
- `cmake ..`
- `make`
    - check if this `LLVM pass` is compiled properly
- cd back here to this folder.
- modify line 3 of Makefile to point to the `bin` folder of `llvm-clang-12.0.0` toolchain that you downloaded and unzipped. 
- `make instrumented_ub`
- execute instrumented_ub by `./instrumented_ub`. You can try to input different integer to see if the integer overflow is intercepted or not if it were to happen. 

## APIs that you are gonna be using:
- CreateSExt:
    1. https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a5253081f40ffcad9c0e8ba6a5c437006
    2. Sign Extent `V` to the target type `DestTy`

- CreateAdd:
    1. https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#aada1d6d8de104a5cd1cb9a02c676cc6c
    2. add `LHS` and `RHS` together.

- CreateCall:
    1. https://llvm.org/doxygen/classllvm_1_1IRBuilderBase.html#a2697fb3ba010f79193b9942081a2cbb7
    2. pass the function (which are already in the Pass Class), and pass in the arguments as arrays, returns a CallInst*