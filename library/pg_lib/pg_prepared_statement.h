//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_PG_PREPARED_STATEMENT_H
#define POSTGRESQL_PG_PREPARED_STATEMENT_H

#include <vector>
#include <libpq-fe.h>
#include "pg_statement.h"
#include "pg_resultset.h"
#include "../tools/date.h"

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
    std::vector<const char *> parameters;
    std::vector<std::string> param_type;
    std::string prepare_prefix;
    std::string prepared_name;
public:
    std::string execute_sql;
    pg_prepared_statement(PGconn *conn, std::string &sql, const parameter_type types[]);
    virtual ~pg_prepared_statement();

    void set_value(int idx, const char *parameter);
    void set_int(int idx, int parameter);
    void set_float(int idx, float parameter);
    void set_date(int idx, PG::Date parameter);
    int execute_update();
    int get_parameters_count();
    std::string get_prepared_name();
    pg_resultset execute_query();
    pg_resultset analyse_sql();
};


#endif //POSTGRESQL_PG_PREPARED_STATEMENT_H
