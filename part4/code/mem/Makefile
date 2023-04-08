PROJ_DIR := $(abspath .)
GCC_PATH := gcc


all: a.out
a.out: mem.o wrapped_malloc.o
	$(GCC_PATH) mem.o wrapped_malloc.o -o $@
%.o: %.c
	$(GCC_PATH)  -g  -c -o $@ $<


clean:
	rm *.o
	rm a.out