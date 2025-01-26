cmake_minimum_required(VERSION 3.10)
project(dbscanTest)
set(TARGET dbscanTest CACHE STRING "Target to compile.")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror=return-type -Wl,--as-needed,--gc-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror=return-type -Wl,--as-needed,--gc-sections")

#是否生成调试版本
# set(CMAKE_BUILD_TYPE "Debug")
# add_definitions(-O0 -g -funwind-tables -rdynamic)
# set(CMAKE_BUILD_TYPE "Release")

#添加引用目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    # ${OpenCV_INCLUDE_DIRS}
)

#添加库路径
link_directories(
    ${PROJECT_SOURCE_DIR}/lib
)

add_executable(dbscanTest main.cpp)

#添加库            
target_link_libraries(dbscanTest pthread)