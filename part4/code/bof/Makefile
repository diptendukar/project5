PROJ_DIR := $(abspath .)
LLVM_PASS_PATH := $(PROJ_DIR)/../../pass/bofsan/build/libbofsan.so
LLVM_BIN_PATH := /home/diptendu/Downloads/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/bin
GCC_PATH := gcc

all: instrumented_bof
SRC_LEN_ := $(SRC_LEN)
instrumented_bof: bof.c
	$(LLVM_BIN_PATH)/clang -DSRC_LEN=$(SRC_LEN_) -g  -emit-llvm ./bof.c -c -o ./bof.bc
	$(LLVM_BIN_PATH)/opt -load $(LLVM_PASS_PATH) -bofsan -o instrumented_bof.bc bof.bc
	$(LLVM_BIN_PATH)/llc -filetype=obj instrumented_bof.bc -o instrumented_bof.o
	gcc instrumented_bof.o -no-pie -o instrumented_bof

normal: bof.c
	gcc bof.c -DSRC_LEN=$(SRC_LEN_)  -g -o bof

clean:
	rm ./bof.bc instrumented_bof.bc instrumented_bof.o instrumented_bof bof *.ll

