//
// Created by Administrator on 2019/3/26.
//

#include "pg_prepared_statement.h"
#include "exception/statement_exception.h"
#include <regex>
#include <time.h>
#include <random>
#include <iostream>
#include "exception/sqlexecute_exception.h"
#include "../tools/global_tools.h"
#include <cstring>

std::default_random_engine e;

pg_prepared_statement::pg_prepared_statement(PGconn *conn,
        std::string &sql, parameter_type types[]): pg_statement(conn) {
    int count = 0;
    std::regex pattern(R"pattern(\$[0-9]*)pattern");
    this->sql = sql;
    std::sregex_iterator iter(sql.begin(), sql.end(), pattern);
    std::sregex_iterator end;
    for (; iter != end; iter ++, count ++);
    if (count <= 0){
        throw statement_exception("The parameter count is invaild");
    }
    this->parameters_count = count;
    this->parameters = std::vector<const char *>(count);
    this->param_type = std::vector<std::string>(count);
    long current_time = e();
    std::vector<int> tmp;
    while(true){
        int tmp_idx = current_time % 10;
        current_time /= 10;
        tmp.push_back(tmp_idx);
        if (current_time == 0){
            break;
        }
    }
    char *name = new char[tmp.size() + 1];
    int i = 0;
    for (std::vector<int>::reverse_iterator iter = tmp.rbegin(); iter != tmp.rend(); iter ++){
        name[i ++] = *iter + 65;
    }
    name[tmp.size()] = 0;
    this->prepared_name = std::string(name);
    this->prepare_prefix = std::string("prepare ") + this->prepared_name + std::string("(");
    delete []name;

    for (int idx = 0; idx < count; idx ++){
        parameter_type type = *(types + idx);
        switch (type){
            case date_type:
                this->param_type[idx] = std::string("date");
                break;
            case int_type:
                this->param_type[idx] = std::string("int");
                break;
            case text_type:
                this->param_type[idx] = std::string("text");
                break;
            case numeric_type:
                this->param_type[idx] = std::string("numeric");
                break;
            case bool_type:
                this->param_type[idx] = std::string("bool");
                break;
            default:
                throw statement_exception("Invaild parameter type");
        }
    }
    for (std::vector<std::string>::iterator iter = this->param_type.begin();
         iter != this->param_type.end(); iter ++){
        if ((iter + 1) == this->param_type.end()){
            this->prepare_prefix += (*iter);
        }else{
            this->prepare_prefix += ((*iter) + std::string(","));
        }
    }
    this->prepare_prefix += (std::string(") as ") + this->sql);
    PGresult *result = PQexec(this->conn, this->prepare_prefix.c_str());
    PQclear(result);
}

void pg_prepared_statement::set_int(int idx, int parameter) {
    if (idx >= this->parameters_count){
        throw statement_exception("The idx is bigger than the biggest parameter count");
    }
    std::string param = parseInt(parameter);
    char *tmp = new char[strlen(param.c_str()) + 1];
    strcpy(tmp, param.c_str());
    tmp[strlen(param.c_str())] = 0;
    this->parameters[idx] = tmp;
}

void pg_prepared_statement::set_float(int idx, float parameter) {
    if (idx >= this->parameters_count){
        throw statement_exception("The idx is bigger than the biggest parameter count");
    }
    char buffer[100];
    sprintf(buffer, "%.5f", parameter);
    char *tmp = new char[strlen(buffer) + 1];
    strcpy(tmp, buffer);
    tmp[strlen(buffer)] = 0;
    this->parameters[idx] = tmp;
}

void pg_prepared_statement::set_value(int idx, const char *parameter) {
    if (idx >= this->parameters_count){
        throw statement_exception("The idx is bigger than the biggest parameter count");
    }
    std::string param = get_pg_string(parameter);
    char *tmp = new char[strlen(param.c_str()) + 1];
    strcpy(tmp, param.c_str());
    tmp[strlen(param.c_str())] = 0;
    this->parameters[idx] = tmp;
}

void pg_prepared_statement::execute_update() {
    std::string execute = std::string("execute ") + this->prepared_name + std::string("(");
    for (std::vector<const char *>::iterator iter = this->parameters.begin();
         iter != this->parameters.end(); iter ++){
        if (!(*iter)){
            throw statement_exception("The parameter count is not enough");
        }
        if ((iter + 1) == this->parameters.end()){
            execute += std::string(*iter);
        }else{
            execute += (std::string(*iter) + std::string(","));
        }
    }
    execute += std::string(");");
    PGresult *res = PQexec(this->conn, execute.c_str());
    this->verify_sql_executeresult(res);
    PQclear(res);
    for (int i = 0; i < this->parameters.size(); i ++){
        if (this->parameters[i]){
            delete []this->parameters[i];
        }
    }
}

pg_resultset pg_prepared_statement::execute_query() {
    std::string execute = std::string("execute ") + this->prepared_name + std::string("(");
    for (std::vector<const char *>::iterator iter = this->parameters.begin();
         iter != this->parameters.end(); iter ++){
        if (!(*iter)){
            throw statement_exception("The parameter count is not enough");
        }
        if ((iter + 1) == this->parameters.end()){
            execute += std::string(*iter);
        }else{
            execute += (std::string(*iter) + std::string(","));
        }
    }
    execute += std::string(");");
    PGresult *result_set = PQexec(this->conn, execute.c_str());
    this->verify_sql_executeresult(result_set);
    for (int i = 0; i < this->parameters.size(); i ++){
        if (this->parameters[i]){
            delete []this->parameters[i];
        }
    }
    return pg_resultset(result_set);
}

pg_prepared_statement::~pg_prepared_statement() {
    std::string execute = std::string("deallocate prepare ") + this->prepared_name + std::string(";");
    PGresult *result = PQexec(this->conn, execute.c_str());
    PQclear(result);
    for (int i = 0; i < this->parameters.size(); i ++){
        if (this->parameters[i]){
            delete []this->parameters[i];
        }
    }
}


