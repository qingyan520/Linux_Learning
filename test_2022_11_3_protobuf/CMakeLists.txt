cmake_minimum_required(VERSION 3.1)
project(Demo)

#设置项目工程输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#设置项目头文件搜说路径 -I
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#设置项目连接动态库路径 -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

#添加子目录
add_subdirectory(${PROJECT_SOURCE_DIR}/src)