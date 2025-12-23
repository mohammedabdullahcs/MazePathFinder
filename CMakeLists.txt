cmake_minimum_required(VERSION 3.16)
project(MazeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/mazescene.h
    src/mazescene.cpp
    src/maze.h
    src/maze.cpp
    src/unionfind.h
    src/unionfind.cpp
    src/pathfinder.h
    src/pathfinder.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)