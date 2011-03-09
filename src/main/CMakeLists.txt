include_directories (${PROJECT_SOURCE_DIR}/cluster)
add_executable (magister main)

add_library (Reader Reader.cpp)

target_link_libraries (Reader Object)

target_link_libraries (magister Object)
target_link_libraries (magister Cluster)
target_link_libraries (magister Reader)
target_link_libraries (magister DataContainer)
