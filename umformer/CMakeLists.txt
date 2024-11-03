set(PROJECT_NAME umformer)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

add_library(umformer ${SOURCES})


