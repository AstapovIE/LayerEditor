cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME umformer)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(umformer ${SOURCES})

target_include_directories(umformer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")


