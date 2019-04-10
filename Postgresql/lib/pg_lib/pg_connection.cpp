//
// Created by ilmare on 2/24/19.
//

#include "pg_connection.h"
#include "exception/conn_info_nullpointer_exception.h"
#include "exception/conn_fail_exception.h"
#include "exception/sqlexecute_exception.h"
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

pg_connection::pg_connection(const pg_connection &obj) {
    if (obj.conn_info == NULL || !strlen(obj.conn_info)){
        throw conn_info_nullpointer_exception();
    }
    this->conn_info = new char[strlen(obj.conn_info) + 1];
    strcpy(this->conn_info, obj.conn_info);
    this->pg_conn = PQconnectdb(this->conn_info);
    this->connection_status = PQstatus(this->pg_conn);
    if (this->connection_status != CONNECTION_OK){
        PQfinish(this->pg_conn);
        this->pg_conn = NULL;
        throw conn_fail_exception(this->connection_status);
    }
}

pg_connection::pg_connection(const char *connection_info) {
    if (connection_info == NULL || !strlen(connection_info)){
        throw conn_info_nullpointer_exception();
    }
    this->conn_info = new char[strlen(connection_info) + 1];
    strcpy(this->conn_info, connection_info);
    this->pg_conn = PQconnectdb(this->conn_info);
    this->connection_status = PQstatus(this->pg_conn);
    if (this->connection_status != CONNECTION_OK){
        PQfinish(this->pg_conn);
        this->pg_conn = NULL;
        throw conn_fail_exception(this->connection_status);
    }
}

pg_connection::~pg_connection() {
    PGresult *result = PQexec(this->pg_conn, "deallocate prepare all;");
    PQclear(result);
    delete [] this->conn_info;
    this->conn_info = NULL;
    this->close();
}

pg_connection::pg_connection(const char *user_name, const char *password,
        const char *host_name, const char *database,
        const char *port, int connection_timeout) {
    if (user_name == NULL || password == NULL ||
    host_name == NULL || database == NULL || port == NULL){
        throw conn_info_nullpointer_exception();
    }
    std::vector<int> val;
    while(true){
        int idx = connection_timeout % 10;
        val.push_back(idx);
        connection_timeout /= 10;
        if (!connection_timeout){
            break;
        }
    }
    char *timeout = new char[val.size() + 1];
    int i = 0;
    for (std::vector<int>::reverse_iterator iter = val.rbegin(); iter != val.rend(); iter ++){
        timeout[i ++] = *iter + 48;
    }
    timeout[i] = 0;
    std::string tmp = "postgresql://" + std::string(user_name) +
            "@" + std::string(host_name) + "/" + std::string(database) +
            "?connect_timeout=" + std::string(timeout) + "&password=" + std::string(password);
    this->conn_info = new char[tmp.length() + 1];
    strcpy(this->conn_info, tmp.c_str());
    this->conn_info[tmp.length()] = 0;
    delete []timeout;
    this->pg_conn = PQconnectdb(this->conn_info);
    this->connection_status = PQstatus(this->pg_conn);
    if (this->connection_status != CONNECTION_OK){
        PQfinish(this->pg_conn);
        this->pg_conn = NULL;
        throw conn_fail_exception(this->connection_status);
    }
}

pg_statement pg_connection::create_statement() {
    pg_statement statement = pg_statement(this->pg_conn);
    return statement;
}

void pg_connection::close() {
    if (this->pg_conn){
        PQfinish(this->pg_conn);
        this->pg_conn = NULL;
    }
}

void pg_connection::set_auto_commit(bool flag) {
    this->auto_commit = flag;
    if (this->auto_commit){
        return;
    }
    PGresult *result = PQexec(this->pg_conn, "BEGIN;");
    ExecStatusType type = PQresultStatus(result);
    switch (type){
        case PGRES_EMPTY_QUERY:
            throw sqlexecute_exception(type);
        case PGRES_BAD_RESPONSE:
            throw sqlexecute_exception(type);
        case PGRES_NONFATAL_ERROR:
            throw sqlexecute_exception(type);
        case PGRES_FATAL_ERROR:
            throw sqlexecute_exception(type);
        default:
            std::clog << "TRANSACTION BEGIN" << std::endl;
            break;
    }
}

void pg_connection::roll_back() {
    if (!this->auto_commit){
        PGresult *result = PQexec(this->pg_conn, "ROLLBACK;");
        ExecStatusType type = PQresultStatus(result);
        switch (type){
            case PGRES_EMPTY_QUERY:
                throw sqlexecute_exception(type);
            case PGRES_BAD_RESPONSE:
                throw sqlexecute_exception(type);
            case PGRES_NONFATAL_ERROR:
                throw sqlexecute_exception(type);
            case PGRES_FATAL_ERROR:
                throw sqlexecute_exception(type);
            default:
                std::clog << "TRANSACTION ROLLBACK" << std::endl;
                break;
        }
    }
}

void pg_connection::commit() {
    if (!this->auto_commit){
        PGresult *result = PQexec(this->pg_conn, "COMMIT;");
        ExecStatusType type = PQresultStatus(result);
        switch (type){
            case PGRES_EMPTY_QUERY:
                throw sqlexecute_exception(type);
            case PGRES_BAD_RESPONSE:
                throw sqlexecute_exception(type);
            case PGRES_NONFATAL_ERROR:
                throw sqlexecute_exception(type);
            case PGRES_FATAL_ERROR:
                throw sqlexecute_exception(type);
            default:
                std::clog << "TRANSACTION COMMIT" << std::endl;
                break;
        }
    }
}

pg_prepared_statement pg_connection::prepared_statement(std::string &sql, parameter_type types[]) {
    pg_prepared_statement st = pg_prepared_statement(this->pg_conn, sql, types);
    return st;
}

pg_connection& pg_connection::operator=(const pg_connection &obj) {
    if (this == &obj){
        return *this;
    }
    if (obj.conn_info == NULL || !strlen(obj.conn_info)){
        throw conn_info_nullpointer_exception();
    }
    delete []this->conn_info;
    this->conn_info = new char[strlen(obj.conn_info) + 1];
    strcpy(this->conn_info, obj.conn_info);
    this->pg_conn = PQconnectdb(this->conn_info);
    this->connection_status = PQstatus(this->pg_conn);
    if (this->connection_status != CONNECTION_OK){
        PQfinish(this->pg_conn);
        throw conn_fail_exception(this->connection_status);
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const pg_connection &connection){
    os << "pg_connection@" << (void *)&connection << ":" << connection.conn_info;
    return os;
}