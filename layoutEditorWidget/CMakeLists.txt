cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME layoutEditorWidget)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(layoutEditorWidget ${SOURCES})

target_include_directories(layoutEditorWidget PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(layoutEditorWidget
    Qt6::Widgets)

