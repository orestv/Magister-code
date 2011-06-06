add_library (Attribute Attribute.cpp)
add_library (Object Object.cpp)
target_link_libraries (Object Attribute)


add_library (Cluster Cluster.cpp)
target_link_libraries (Cluster DataContainer)

add_library(Clustering clustering.cpp)
target_link_libraries(Clustering Cluster)

add_library (DataContainer DataContainer.cpp)
target_link_libraries (DataContainer Object)

add_library (KMeans KMeans.cpp)
target_link_libraries (KMeans DataContainer)
target_link_libraries (KMeans Cluster)
target_link_libraries (KMeans Clustering)

add_library(Upgma Upgma.cpp)
target_link_libraries (Upgma DataContainer)
target_link_libraries (Upgma Cluster)
target_link_libraries (Upgma Clustering)

add_library (EuclidMetric EuclidMetric.cpp)
target_link_libraries (EuclidMetric Regression)

add_library(NN nn.cpp)
target_link_libraries (NN DataContainer)
target_link_libraries (NN Cluster)
target_link_libraries (NN Clustering)

add_library(DBScan dbscan.cpp)
target_link_libraries (DBScan DataContainer)
target_link_libraries (DBScan Cluster)
target_link_libraries (DBScan Clustering)


add_library (Validity validity.cpp)
target_link_libraries (Validity Clustering)
