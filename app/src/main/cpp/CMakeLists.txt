cmake_minimum_required(VERSION 3.4.1)
add_library(VbusSimulator SHARED
            connect.cpp
            interface.cpp)

target_link_libraries(VbusSimulator
                       android
                       log)