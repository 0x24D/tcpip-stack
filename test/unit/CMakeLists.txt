project(tcpip-stack)

add_compile_definitions(FCS_CAPTURED)

add_executable(test-unit-ethernet ethernet.cpp ${CMAKE_BINARY_DIR}/src/ethernet.cpp)
