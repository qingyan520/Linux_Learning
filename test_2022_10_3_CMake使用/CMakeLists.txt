cmake_minimum_required (VERSION 2.8)

#项目生成文件
project(Demo3)

#查找当前目录下所有源文件
#并且保存到DIR变量
aux_source_directory(. IDR)

#添加math子目录
add_subdirectory(math)

#指定生成目标
add_executable(Demo3 main.cpp)

#添加链接库
target_link_libraries(Demo3 MathFunctions)


