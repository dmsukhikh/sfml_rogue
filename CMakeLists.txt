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
                 src/Settings.cpp
                 src/MapManager.cpp)

add_library(entities src/entities/Entity.cpp
                     src/entities/Wall.cpp
                     src/entities/Gamer.cpp
                     src/entities/Floor.cpp
                     src/entities/Port.cpp
                     src/entities/Movable.cpp
                     src/entities/Bullets.cpp
                     src/entities/AbstractEnemy.cpp
                     src/entities/Striker.cpp
                     src/entities/Lazer.cpp
                     src/entities/Sniper.cpp
                     src/entities/Bigboy.cpp
                     src/entities/Wolf.cpp
                     src/entities/Fireball.cpp
                     src/entities/Wizard.cpp
                     src/entities/Item.cpp
                     src/entities/UltCharge.cpp)

add_library(vecmath src/vecmath.cpp)



target_include_directories(dependencies_interface INTERFACE include 
                                                            include/gamegui
                                                            include/entities)
target_link_libraries(dependencies_interface INTERFACE
                      sfml-system sfml-window sfml-graphics sfml-audio)

target_link_libraries(entities PUBLIC vecmath)
target_link_libraries(dependencies_interface INTERFACE base gamegui entities)

add_executable(main src/main.cpp)
target_link_libraries(main dependencies_interface)
