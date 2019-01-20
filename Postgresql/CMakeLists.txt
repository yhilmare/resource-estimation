cmake_minimum_required(VERSION 3.12)
project(Postgresql)

set(CMAKE_CXX_STANDARD 11)

set(PGSQL_INCLUDE "/home/ilmare/Downloads/pgsql/include")

set(PGSQL_LIB "/home/ilmare/Downloads/pgsql/lib")

include_directories(${PGSQL_INCLUDE})

link_directories(${PGSQL_LIB})

add_executable(Postgresql main.cpp)

target_link_libraries(Postgresql ${PGSQL_LIB}/libpq.so)