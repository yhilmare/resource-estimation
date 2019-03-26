//
// Created by Administrator on 2019/3/25.
//

#ifndef POSTGRESQL_PG_STATEMENT_H
#define POSTGRESQL_PG_STATEMENT_H

#include <string>

class pg_statement {
private:
    std::string sql;
public:
    pg_statement(){};

    void execute_update();

};

#endif //POSTGRESQL_PG_STATEMENT_H
