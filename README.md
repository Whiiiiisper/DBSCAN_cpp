# DBSCAN_cpp
Header-Only DBSCAN for C++, which is using kdtree and threadpool, and is allowed to customize distance function.

based on work of https://github.com/Lallapallooza/clustering.git

This algorithm costs 3~4s in my pc with a dataset contains 33471 points.

## Example
You can test this DBSCAN algorithm with example code(main.cpp) & sample data(frames_data_full.txt).
To add a custom distance:
    1. modify the template parameter(KDTreeDistanceType) of the dbscan class. (dbscan.h)
    2. add a new type in enum class KDTreeDistanceType. (kdtree.h)
    3. add your custom distance function to distance function. (kdtree.h)

## TODO
- incremental dbscan