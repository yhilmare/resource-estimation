//
// Created by Administrator on 2019/3/25.
//

#include "pg_statement.h"
#include "exception/statement_exception.h"

pg_statement::pg_statement(PGconn *conn) {
    if (conn == NULL){
        throw statement_exception("The PGconn is NULL");
    }
    this->conn = conn;
}

void pg_statement::execute_update(const std::string &sql) {
    this->sql = sql;
    PQexec(this->conn, this->sql.c_str());
}

pg_resultset pg_statement::execute_query(const std::string &sql) {
    this->sql = sql;
    PGresult *result_set = PQexec(this->conn, this->sql.c_str());
    return pg_resultset(result_set);
}

