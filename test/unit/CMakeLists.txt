project(tcpip-stack)

add_executable(test-unit-arp arp.cpp ${CMAKE_BINARY_DIR}/src/arp.cpp)

add_compile_definitions(FCS_CAPTURED)
add_executable(test-unit-ethernet ethernet.cpp ${CMAKE_BINARY_DIR}/src/arp.cpp ${CMAKE_BINARY_DIR}/src/ethernet.cpp)
