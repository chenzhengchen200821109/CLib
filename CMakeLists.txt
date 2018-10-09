cmake_minimum_required(VERSION 3.5)
project(CLib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES table.c assert_v.c except_v.c mem.c mem_product.c array.c seq.c seq_test.c ring.c ring.h sort.c thread.c thread.h spin.c sem.c sem.h text.c text.h chan.c chan.h sieve.c)

add_executable(main ${SOURCE_FILES})