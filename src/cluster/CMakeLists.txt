#add_library (Attribute Attribute.cpp)

add_library (Object Object.cpp)
#target_link_libraries (Object Attribute)


add_library (Cluster Cluster.cpp)
target_link_libraries (Cluster DataContainer)

add_library (DataContainer DataContainer.cpp)
target_link_libraries (DataContainer Object)

add_library (KMeans KMeans.cpp)
target_link_libraries (KMeans DataContainer)
target_link_libraries (KMeans Cluster)

add_library(Upgma Upgma.cpp)
target_link_libraries (Upgma DataContainer)
target_link_libraries (Upgma Cluster)

add_library (EuclidMetric EuclidMetric.cpp)
target_link_libraries (EuclidMetric Regression)
