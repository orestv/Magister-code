cmake_minimum_required (VERSION 2.6)
project (Magister)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -pthread")
set (EXECUTABLE_OUTPUT_PATH "../bin/")
set (CMAKE_BUILD_TYPE, "Debug")

set (PROJECT_VERSION_MAJOR, 1)
set (PROJECT_VERSION_MINOR, 0)

add_subdirectory (main)
add_subdirectory (cluster)
