cmake_minimum_required(VERSION 3.13)
project(test)

subdirs(emulator)

link_directories(${CMAKE_CURRENT_LIST_DIR}/emulator)
add_executable(test test.cpp)
target_link_libraries(test emulator)