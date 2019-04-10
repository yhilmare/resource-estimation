//
// Created by Administrator on 2019/3/26.
//

#include "pg_resultset.h"
#include "exception/resultset_nullpointer_exception.h"
#include <iostream>

pg_resultset& pg_resultset::operator=(const pg_resultset &res) {
    if (this == &res){
        return *this;
    }
    this->close();
    this->result_set = res.result_set;
    this->column_count = res.column_count;
    this->row_count = res.row_count;
    this->cursor = -1;
    return *this;
}

pg_resultset::pg_resultset(const pg_resultset &res) {
    this->close();
    this->result_set = res.result_set;
    this->column_count = res.column_count;
    this->row_count = res.row_count;
    this->cursor = -1;
}

pg_resultset::pg_resultset(PGresult *res): result_set(res), cursor(-1){
    if (this->result_set == NULL){
        throw resultset_nullpointer_exception(std::string("The resultset is NULL"));
    }
    this->column_count = PQnfields(this->result_set);
    this->row_count = PQntuples(this->result_set);
}

void pg_resultset::close() {
    if (this->result_set){
        PQclear(this->result_set);
        this->result_set = NULL;
    }
}

pg_resultset::~pg_resultset() {
    this->close();
}

std::string pg_resultset::get_field(int idx) {
    if (idx >= get_column_count()){
        throw resultset_nullpointer_exception(std::string("idx is bigger than the biggest column count"));
    }
    return PQfname(this->result_set, idx);
}

char* pg_resultset::get_value(int idx) {
    if (idx >= this->column_count){
        throw resultset_nullpointer_exception(std::string("idx is bigger than the biggest column count"));
    }
    if (this->cursor < this->get_tuples_count()){
        return PQgetvalue(this->result_set, this->cursor, idx);
    }else{
        throw resultset_nullpointer_exception(std::string("cursor is bigger than row count"));
    }
}
int pg_resultset::get_tuples_count() {
    return this->row_count;
}

int pg_resultset::get_column_count() {
    return this->column_count;
}

bool pg_resultset::has_next() {
    if (++(this->cursor) < this->row_count){
        return true;
    }
    return false;
}

void pg_resultset::show_with_elegant_format() {
    for (int i = 0; i < this->get_column_count(); i ++){
        printf("%-15s", this->get_field(i).c_str());
    }
    std::cout << std::endl;
    while(this->has_next()){
        for (int i = 0; i < this->get_column_count(); i ++){
            printf("%-15s", this->get_value(i));
        }
        std::cout << std::endl;
    }
}

