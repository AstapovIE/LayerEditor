cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME processDialog)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "processordialog.ui")

add_library(processDialog ${SOURCES})

target_include_directories(processDialog PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(processDialog
	Qt5::Core
	Qt5::Widgets
	Qt5::Sql)

#add_executable(${PROJECT_NAME} ${source_list})
