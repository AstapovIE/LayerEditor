cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME HelpSystem)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(HelpSystem ${SOURCES})

target_include_directories(HelpSystem PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(HelpSystem
    Qt5::Core
    Qt5::Widgets)

#add_executable(${PROJECT_NAME} ${source_list})
