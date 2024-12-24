# 指定CMake的最低版本要求为3.22
cmake_minimum_required(VERSION 3.22)

#
# 该文件是cmake调用的主构建文件
# 用户可以根据需要自由修改此文件。
#

# 设置编译器设置部分
set(CMAKE_C_STANDARD 11)            # 设置C标准为C11
set(CMAKE_C_STANDARD_REQUIRED ON)   # 要求使用指定的C标准
set(CMAKE_C_EXTENSIONS ON)          # 启用编译器扩展

set(CMAKE_BUILD_BL $ENV{PROGRAM_BLQ})

# set(CMAKE_BUILD_TYPE "Release")
# 定义构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")   # 如果未设置CMAKE_BUILD_TYPE，则默认设置为"Debug"。该参数可以在使用类似"cmake ../"生成原生构建系统时添加-DCMAKE_BUILD_TYPE=Release指定
endif()

# 包含工具链文件
include("${CMAKE_CURRENT_LIST_DIR}/Example/gcc-arm-none-eabi.cmake")

# 设置项目名称
set(CMAKE_PROJECT_NAME H7_GCC_example)  # 设置项目名称


# 启用编译命令生成，以便于其他工具进行索引例如clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # 生成compile_commands.json，以便IDE或工具使用

# 启用CMake对ASM和C语言的支持
enable_language(C ASM)              # 启用C和汇编（ASM）语言支持

# 核心项目设置
project(${CMAKE_PROJECT_NAME})                  # 定义项目，使用之前设置的项目名称
message("Build type: " ${CMAKE_BUILD_TYPE})     # 消息输出构建类型

# 创建一个可执行对象
add_executable(${CMAKE_PROJECT_NAME})   # 定义一个可执行目标，使用项目名称

# 添加子目录部分，这会自动处理子目录中的CMakeLists.txt文件
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Example/stm32H743)     
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/common)  

# 链接目录设置
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # 添加用户定义的库搜索路径
    # e.g., "/path/to/libs"
)

# 向可执行目标添加源文件
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # 添加额外的源文件
    # e.g., "src/main.c"
)

# 添加包含路径
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # 添加用户定义的包含路径
    # e.g., "include"
)

# 添加项目符号（宏）
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # 添加用户定义的符号
    # e.g., "MY_MACRO=1"
)

# 添加链接库
target_link_libraries(${CMAKE_PROJECT_NAME}
    user_src # 链接user_src库 实际上也是以project()项目的形式存在
    Dataflow
    # 添加用户定义的库
    # e.g., "mylib"
)


# 运行一下构建后任务
include("${CMAKE_CURRENT_LIST_DIR}/Example/post-build-tasks.cmake")
