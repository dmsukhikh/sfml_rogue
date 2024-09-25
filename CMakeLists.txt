cmake_minimum_required(VERSION 3.6)
project('rogue')
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_package(SFML COMPONENTS system window graphics audio REQUIRED)
add_library(dependencies_interface INTERFACE)
target_include_directories(dependencies_interface INTERFACE include)
target_link_libraries(dependencies_interface INTERFACE
                      sfml-system sfml-window sfml-graphics sfml-audio)

add_executable(main src/main.cpp)
target_link_libraries(main dependencies_interface)
