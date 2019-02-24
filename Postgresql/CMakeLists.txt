cmake_minimum_required(VERSION 3.12)
project(Postgresql)

set(CMAKE_CXX_STANDARD 11)

set(PGSQL_INCLUDE "/home/ilmare/Downloads/pgsql/include")

set(PGSQL_LIB "/home/ilmare/Downloads/pgsql/lib")

include_directories(${PGSQL_INCLUDE})

link_directories(${PGSQL_LIB})

add_executable(Postgresql main.cpp lib/pg_lib/pg_connection.cpp lib/pg_lib/pg_connection.h lib/pg_lib/exception/conn_info_nullpointer_exception.cpp lib/pg_lib/exception/conn_info_nullpointer_exception.h lib/pg_lib/exception/conn_fail_exception.cpp lib/pg_lib/exception/conn_fail_exception.h)

target_link_libraries(Postgresql ${PGSQL_LIB}/libpq.so)