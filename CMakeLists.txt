cmake_minimum_required(VERSION 3.20)

project(tcpip-stack)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include_directories(include)

add_compile_options(-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wpedantic)

add_subdirectory(src)
add_subdirectory(test)
