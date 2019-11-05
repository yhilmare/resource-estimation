//
// Created by Administrator on 2019/3/25.
//

#ifndef POSTGRESQL_PG_STATEMENT_H
#define POSTGRESQL_PG_STATEMENT_H

#include <string>
#include <libpq-fe.h>
#include "pg_resultset.h"

class pg_statement {
protected:
    std::string sql;
    PGconn *conn;
    int effect_num;
    void verify_sql_executeresult(PGresult *);
public:
    pg_statement(PGconn *conn);

    int get_effect_count();
    virtual int execute_update(const std::string &);
    virtual pg_resultset execute_query(const std::string &);
};

#endif //POSTGRESQL_PG_STATEMENT_H
