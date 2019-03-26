//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_PG_PREPARED_STATEMENT_H
#define POSTGRESQL_PG_PREPARED_STATEMENT_H

#include "pg_statement.h"
#include <libpq-fe.h>
#include <vector>

class pg_prepared_statement: public pg_statement {
private:
    int parameters_count;
    std::vector<char *> parameters;
public:
    pg_prepared_statement(PGconn *conn, int param_count, std::string &sql);

    void set_value(char *parameter, int idx);
};


#endif //POSTGRESQL_PG_PREPARED_STATEMENT_H
