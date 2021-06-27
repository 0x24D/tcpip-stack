project(tcpip-stack)

add_executable(tcpip-stack main.cpp
                           ethernet.cpp
                           taptun.cpp)
