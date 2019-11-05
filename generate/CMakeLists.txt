cmake_minimum_required(VERSION 3.5)

project(generate)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY_INCLUDE "/home/ilmare/workspace/Resource_estimation/library")

set(PG_INCLUDE "/usr/include/postgresql")

set(LIBRARY_LIB "/usr/lib")

include_directories(${LIBRARY_INCLUDE} ${PG_INCLUDE})

link_directories(${LIBRARY_LIB})

add_executable(generate main.cpp load/data_load.cpp)

target_link_libraries(generate library pq)