cmake_minimum_required(VERSION 3.5)

project(load)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY_INCLUDE "/home/ilmare/workspace/Resource_estimation/library")

set(PG_INCLUDE "/usr/include/postgresql")

set(LIBRARY_LIB "/usr/lib")

include_directories(${LIBRARY_INCLUDE} ${PG_INCLUDE})

link_directories(${LIBRARY_LIB})

add_executable(load main.cpp load/data_load.cpp
        load/driver.cpp tpcc/sequence.cpp tpcc/neword.cpp
        tpcc/payment.cpp tpcc/ordstat.cpp tpcc/delivery.cpp tpcc/slev.cpp
        tpcc/container/transaction_obj.cpp tpcc/container/transaction_obj.h)

target_link_libraries(load library pq pthread)