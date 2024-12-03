file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(HelpSystem ${SOURCES})

target_include_directories(HelpSystem PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(HelpSystem
        Qt6::Core
        Qt6::Widgets
)
