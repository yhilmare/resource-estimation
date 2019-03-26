//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_PG_RESULTSET_H
#define POSTGRESQL_PG_RESULTSET_H

#include <libpq-fe.h>

class pg_resultset {
private:
    const PGresult *result_set;
    int row_count;
    int column_count;
public:
    pg_resultset(PGresult *res);

};


#endif //POSTGRESQL_PG_RESULTSET_H
