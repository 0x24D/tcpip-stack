project(tcpip-stack)

add_executable(test-unit-ethernet ethernet.cpp ${CMAKE_BINARY_DIR}/src/ethernet.cpp)
