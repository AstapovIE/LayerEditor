cmake_minimum_required(VERSION 3.16)

project(LayerEditor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        REQUIRED)


add_executable(LayerEditor
        mainwindow.h
        main.cpp
        mainwindow.cpp
        mainwindow.ui
      )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/database)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/processDialog)
include_directories(${Qt5_INCLUDE_DIR})

target_include_directories(LayerEditor PRIVATE database/src processDialog/src)

target_link_libraries(LayerEditor
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
        database
        processDialog
        )
