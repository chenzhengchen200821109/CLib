# Set cflags 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -Wall -fPIC")

# include directory
include_directories(../include)

# Add source files 
set(SOURCE_FILES list.c
                 stack.c
                 mem.c
                 assert_v.c
                 except_v.c
                 error.c
                 table.c
                 thread.c
                 swtch.S
                 array.c)
# Add static and shared library target 
add_library(clib_static STATIC ${SOURCE_FILES}) 
#add_library(clib_shared SHARED ${SOURCE_FILES}) ${DIR_LIB_SRCS})
