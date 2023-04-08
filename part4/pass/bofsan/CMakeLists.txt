cmake_minimum_required(VERSION 3.0)
project(BofSanPasses)
set(llvm_hints "/home/diptendu/Downloads/clang+llvm-12.0.0-x86_64-linux-gnu-ubuntu-20.04/lib/cmake")

find_package(LLVM REQUIRED CONFIG HINTS ${llvm_hints})

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
include_directories(${LLVM_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti -g")


add_library(bofsan MODULE bofsan.cpp)
