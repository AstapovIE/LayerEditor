# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\LayerEditor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\LayerEditor_autogen.dir\\ParseCache.txt"
  "LayerEditor_autogen"
  )
endif()
