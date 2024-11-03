file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(layoutEditorWidget ${SOURCES})

target_include_directories(layoutEditorWidget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR} umformer/src)
target_link_libraries(layoutEditorWidget
	Qt6::Widgets
	umformer
)

