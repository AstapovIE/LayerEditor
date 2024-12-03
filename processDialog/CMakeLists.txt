file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h" "processordialog.ui")

add_library(processDialog ${SOURCES})

target_include_directories(processDialog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(processDialog
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
)
