cmake_minimum_required(VERSION 3.6)
project('rogue')
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_package(SFML COMPONENTS system window graphics audio REQUIRED)

add_library(dependencies_interface INTERFACE)
add_library(gamegui src/gamegui/GuiObject.cpp
                    src/gamegui/Button.cpp
                    src/gamegui/AbsolutePacker.cpp
                    src/gamegui/AbstractPacker.cpp
                    src/gamegui/GridPacker.cpp
                    src/gamegui/Image.cpp
                    src/gamegui/Slider.cpp
                    src/gamegui/Label.cpp)

add_library(base src/Game.cpp
                 src/Settings.cpp)

target_include_directories(dependencies_interface INTERFACE include 
                                                            include/gamegui)
target_link_libraries(dependencies_interface INTERFACE
                      sfml-system sfml-window sfml-graphics sfml-audio)
target_link_libraries(dependencies_interface INTERFACE gamegui base)

add_executable(main src/main.cpp)
target_link_libraries(main dependencies_interface)
