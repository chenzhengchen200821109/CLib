#在linux平台下使用CMake生成Makefile并编译的流程如下：
#   1.编写CMake配置文件CMakeLists.txt。
#   2.执行命令cmake PATH或者ccmake PATH生成Makefile.
#     ccmake 和 cmake 的区别在于前者提供了一个交互式的界面。
#     其中，PATH是CMakeLists.txt所在的目录。
#   3.使用 make 命令进行编译。

cmake_minimum_required(VERSION 2.8) 

#项目信息
project(clib)

# Set lib version 
set(CLIB_VERSION 0.5)

# Set cflags 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall")

# Use c and asm 
enable_language(C ASM)

# 添加子目录
add_subdirectory(lib)

# include directory
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Macro for add test target 
macro(add_test_target EXAMPLE_TARGET) 
    # 指定生成目标
    add_executable("${EXAMPLE_TARGET}" "${EXAMPLE_TARGET}.c")
    # 添加链接库 
    target_link_libraries("${EXAMPLE_TARGET}" clib_static) 
endmacro(add_test_target)

add_test_target(list_test) 
add_test_target(stack_test)
add_test_target(thread_test)
add_test_target(table_test)
add_test_target(array_test)
