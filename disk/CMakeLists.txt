cmake_minimum_required(VERSION 3.5)

project(disk)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY_INCLUDE "/home/ilmare/workspace/Resource_estimation/library")

set(PG_INCLUDE "/usr/include/postgresql")

set(LIBRARY_LIB "/usr/lib")

include_directories(${LIBRARY_INCLUDE} ${PG_INCLUDE})

link_directories(${LIBRARY_LIB})

add_executable(disk main.cpp domain/pg_database.cpp domain/pg_database.h factory/objectfactory.cpp factory/objectfactory.h util/databassutil.cpp util/databassutil.h domain/pg_buffer.cpp domain/pg_buffer.h util/bufferutil.cpp util/bufferutil.h util/configUtil.cpp util/configUtil.h util/pageflushutil.cpp util/pageflushutil.h)

target_link_libraries(disk library pq)