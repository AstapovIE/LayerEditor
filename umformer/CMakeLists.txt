file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(umformer ${SOURCES})

target_include_directories(umformer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR})

