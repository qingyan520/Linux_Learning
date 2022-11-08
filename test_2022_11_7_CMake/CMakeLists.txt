cmake_minimum_required(VERSION 2.8)
project(test01)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory(src)