//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_PG_PREPARED_STATEMENT_H
#define POSTGRESQL_PG_PREPARED_STATEMENT_H

#include "pg_statement.h"
#include <libpq-fe.h>
#include <vector>
#include "pg_resultset.h"

enum parameter_type{
    date_type,
    int_type,
    text_type,
    bool_type,
    numeric_type
};

class pg_prepared_statement: public pg_statement {
private:
    int parameters_count;
    std::vector<char *> parameters;
    std::vector<std::string> param_type;
    std::string prepare_prefix;
    std::string prepared_name;
public:
    pg_prepared_statement(PGconn *conn, std::string &sql);

    void set_value(char *parameter, int idx, parameter_type type);
    void execute_update();
    pg_resultset execute_query();
};


#endif //POSTGRESQL_PG_PREPARED_STATEMENT_H
