cmake_minimum_required(VERSION 3.12)

project(Postgresql)

set(CMAKE_CXX_STANDARD 11)

set(PGSQL_INCLUDE "C:/pgsql/include")

set(PGSQL_LIB "C:/pgsql/lib/")

include_directories(${PGSQL_INCLUDE})

link_directories(${PGSQL_LIB})

add_executable(Postgresql main.cpp
        lib/pg_lib/pg_connection.cpp
        lib/pg_lib/exception/conn_info_nullpointer_exception.cpp
        lib/pg_lib/exception/conn_fail_exception.cpp lib/pg_lib/pg_statement.cpp lib/pg_lib/pg_statement.h)

target_link_libraries(Postgresql libpq.dll)