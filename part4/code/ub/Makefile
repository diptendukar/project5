PROJ_DIR := $(abspath .)
LLVM_PASS_PATH := $(PROJ_DIR)/../../pass/ubsan/build/libubsan.so
LLVM_BIN_PATH := <your llvm toolchain dir here>/bin
GCC_PATH := gcc

all: instrumented_ub
instrumented_ub: ub.c
	$(LLVM_BIN_PATH)/clang  -g  -emit-llvm ./ub.c -c -o ./ub.bc
	$(LLVM_BIN_PATH)/opt -load $(LLVM_PASS_PATH) -ubsan -o instrumented_ub.bc ub.bc
	$(LLVM_BIN_PATH)/llc -filetype=obj instrumented_ub.bc -o instrumented_ub.o
	gcc instrumented_ub.o -no-pie -o instrumented_ub

normal: ub.c
	gcc ub.c -g -o ub

clean:
	rm ./ub.bc instrumented_ub.bc instrumented_ub.o instrumented_ub ub *.ll

