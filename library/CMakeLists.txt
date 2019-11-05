cmake_minimum_required(VERSION 3.5)

project(library)

set(CMAKE_CXX_STANDARD 11)

set(PGSQL_INCLUDE "/usr/include/postgresql")

set(PGSQL_LIB "/usr/lib/postgresql/9.5/lib")

include_directories(${PGSQL_INCLUDE})

link_directories(${PGSQL_LIB})

add_library(library pg_lib/pg_connection.cpp
        pg_lib/exception/conn_info_nullpointer_exception.cpp
        pg_lib/exception/conn_fail_exception.cpp
        pg_lib/pg_statement.cpp
        pg_lib/pg_resultset.cpp
        pg_lib/exception/resultset_nullpointer_exception.cpp
        pg_lib/exception/statement_exception.cpp
        pg_lib/pg_prepared_statement.cpp
        pg_lib/exception/sqlexecute_exception.cpp
        tools/date.cpp
        tools/global_tools.cpp
        tools/exceptions/file_exception.cpp)

target_link_libraries(library pq)