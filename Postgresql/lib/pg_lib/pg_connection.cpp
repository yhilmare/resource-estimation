//
// Created by ilmare on 2/24/19.
//

#include "pg_connection.h"
#include "exception/conn_info_nullpointer_exception.h"
#include "exception/conn_fail_exception.h"
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
        throw conn_fail_exception(this->connection_status);
    }
}

pg_connection::pg_connection(char *connection_info) {
    if (connection_info == NULL || !strlen(connection_info)){
        throw conn_info_nullpointer_exception();
    }
    this->conn_info = new char[strlen(connection_info) + 1];
    strcpy(this->conn_info, connection_info);
    this->pg_conn = PQconnectdb(this->conn_info);
    this->connection_status = PQstatus(this->pg_conn);
    if (this->connection_status != CONNECTION_OK){
        PQfinish(this->pg_conn);
        throw conn_fail_exception(this->connection_status);
    }
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
        throw conn_fail_exception(this->connection_status);
    }
    std::cout << this->conn_info << std::endl;
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

pg_connection::~pg_connection() {
    delete [] this->conn_info;
    this->close();
}

pg_statement pg_connection::create_statement() {
    pg_statement statement = pg_statement(this->pg_conn);
    return statement;
}

void pg_connection::close() {
    if (this->pg_conn){
        PQfinish(this->pg_conn);
    }
}

pg_prepared_statement pg_connection::prepared_statement(std::string &sql) {
    pg_prepared_statement st = pg_prepared_statement(this->pg_conn, sql);
    return st;
}