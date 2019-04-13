//
// Created by Administrator on 2019/4/9.
//

#ifndef POSTGRESQL_DRIVER_H
#define POSTGRESQL_DRIVER_H

#include "../global_define.h"
#include "../lib/pg_lib/pg_connection.h"
#include "../lib/pg_lib/pg_prepared_statement.h"
#include <vector>

int driver(pg_connection &,
        std::vector<pg_prepared_statement> &, int);

#endif //POSTGRESQL_DRIVER_H
