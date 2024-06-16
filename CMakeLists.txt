cmake_minimum_required(VERSION 3.10)
project(MyProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置源文件目录
set(SOURCES 
    src/main_test.cpp
    src/tinyxml2.cpp
)

# 设置头文件目录
include_directories(include)

# 添加可执行文件
add_executable(main_test ${SOURCES})
