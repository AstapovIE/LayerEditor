set(PROJECT_NAME database)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(database ${SOURCES})

target_include_directories(database PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(database
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
)
