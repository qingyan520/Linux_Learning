aux_source_directory(. SRC_LIST)
#set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../lib)
add_library(test01 SHARED ${SRC_LIST})