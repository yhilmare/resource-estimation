//
// Created by Administrator on 2019/3/26.
//

#include "pg_prepared_statement.h"
#include "exception/statement_exception.h"
#include <regex>

pg_prepared_statement::pg_prepared_statement(PGconn *conn, int param_count,
        std::string &sql): pg_statement(conn) {
    if (param_count <= 0){
        throw statement_exception("The parameter count is negative");
    }
    int count = 0;
    std::regex pattern(R"pattern(\$[0-9]*)pattern");
    this->sql = sql;
    std::sregex_iterator iter(sql.begin(), sql.end(), pattern);
    std::sregex_iterator end;
    for (; iter != end; iter ++, count ++);
    if (count != parameters_count){
        throw statement_exception("The parameter count is invaild");
    }
    this->parameters_count = param_count;
}
