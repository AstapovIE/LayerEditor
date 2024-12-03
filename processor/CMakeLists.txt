file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_executable(processor ${SOURCES})

target_include_directories(processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})
target_link_libraries(processor umformer)

