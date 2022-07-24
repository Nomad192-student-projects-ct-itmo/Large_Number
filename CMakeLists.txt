cmake_minimum_required(VERSION 3.22)
project(LN)
set(CMAKE_CXX_STANDARD 17)

add_library(my_ptr_unchained STATIC my_ptr_unchained.cpp my_ptr_unchained.h)
add_library(my_logging STATIC my_logging.cpp my_logging.h return_codes.h)
add_library(LN_LIB STATIC LN.cpp LN.h return_codes.h)

add_executable(${PROJECT_NAME} main.cpp return_codes.h)

target_link_libraries(${PROJECT_NAME} my_ptr_unchained)
target_link_libraries(${PROJECT_NAME} my_logging)
target_link_libraries(${PROJECT_NAME} LN_LIB)