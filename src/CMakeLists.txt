project(tcpip-stack)

add_executable(tcpip-stack main.cpp
                           arp.cpp
                           ethernet.cpp
                           taptun.cpp)
