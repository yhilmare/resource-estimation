//
// Created by Administrator on 2019/3/25.
//
#include <iostream>
#include "pg_statement.h"
#include "exception/statement_exception.h"
#include "exception/sqlexecute_exception.h"

pg_statement::pg_statement(PGconn *conn) {
    if (conn == NULL){
        throw statement_exception("The PGconn is NULL");
    }
    this->conn = conn;
}

int pg_statement::execute_update(const std::string &sql) {
    this->sql = sql;
    PGresult *result_set = PQexec(this->conn, this->sql.c_str());
    int tmp = atoi(PQcmdTuples(result_set));
    this->effect_num = tmp;
    this->verify_sql_executeresult(result_set);
    PQclear(result_set);
    return tmp;
}

pg_resultset pg_statement::execute_query(const std::string &sql) {
    this->sql = sql;
    PGresult *result_set = PQexec(this->conn, this->sql.c_str());
    this->effect_num = atoi(PQcmdTuples(result_set));
    this->verify_sql_executeresult(result_set);
    return pg_resultset(result_set);
}

void pg_statement::verify_sql_executeresult(PGresult *res) {
    ExecStatusType type = PQresultStatus(res);
    switch (type){
        case PGRES_EMPTY_QUERY:
            std::cerr << PQresultErrorMessage(res) << std::endl;
            throw sqlexecute_exception(type);
        case PGRES_BAD_RESPONSE:
            std::cerr << PQresultErrorMessage(res) << std::endl;
            throw sqlexecute_exception(type);
        case PGRES_NONFATAL_ERROR:
            std::cerr << PQresultErrorMessage(res) << std::endl;
            throw sqlexecute_exception(type);
        case PGRES_FATAL_ERROR:
            std::cerr << PQresultErrorMessage(res) << std::endl;
            throw sqlexecute_exception(type);
        default:
            break;
    }
}

int pg_statement::get_effect_count() {
    return this->effect_num;
}
