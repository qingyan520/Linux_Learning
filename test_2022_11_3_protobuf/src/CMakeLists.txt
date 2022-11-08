#设置子目录下的源文件目录
aux_source_directory(. SRC_LIST)

#设置目标
add_executable(test01 ${SRC_LIST})

#设置连接的动态库
target_link_libraries(test01 protobuf)