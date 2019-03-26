//
// Created by Administrator on 2019/3/26.
//

#include "pg_resultset.h"
#include "exception/resultset_nullpointer_exception.h"
#include <iostream>

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

