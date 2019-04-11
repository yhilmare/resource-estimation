cmake_minimum_required(VERSION 3.12)

project(Postgresql)

set(CMAKE_CXX_STANDARD 11)

set(PGSQL_INCLUDE "C:/pgsql/simple_include")

set(PGSQL_LIB "C:/pgsql/lib/")

include_directories(${PGSQL_INCLUDE})

link_directories(${PGSQL_LIB})

add_executable(Postgresql main.cpp
        lib/pg_lib/pg_connection.cpp
        lib/pg_lib/exception/conn_info_nullpointer_exception.cpp
        lib/pg_lib/exception/conn_fail_exception.cpp
        lib/pg_lib/pg_statement.cpp
        lib/pg_lib/pg_resultset.cpp
        lib/pg_lib/exception/resultset_nullpointer_exception.cpp
        lib/pg_lib/exception/statement_exception.cpp
        lib/pg_lib/pg_prepared_statement.cpp
        lib/pg_lib/exception/sqlexecute_exception.cpp
        lib/tools/date.cpp
        lib/tools/global_tools.cpp
        lib/tools/exceptions/file_exception.cpp load/data_load.cpp load/driver.cpp load/driver.h tpcc/sequence.cpp tpcc/sequence.h tpcc/neword.cpp tpcc/neword.h)

target_link_libraries(Postgresql libpq.dll)