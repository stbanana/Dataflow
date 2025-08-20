cmake_minimum_required(VERSION 3.22)

project(DATAFLOW)
add_library(Dataflow INTERFACE) # INTERFACE意味着这个库本身并不会被编译，而是作为依赖被其他目标使用，以便于MBx_user.h可以自己定义并且对库行为进行变更

# 递归查找所有源码文件
file(GLOB_RECURSE SRC ${CMAKE_CURRENT_LIST_DIR}/source/*.c)

target_include_directories(Dataflow INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(Dataflow INTERFACE
     ${SRC}
)
