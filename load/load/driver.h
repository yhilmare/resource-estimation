//
// Created by Administrator on 2019/4/9.
//

#ifndef POSTGRESQL_DRIVER_H
#define POSTGRESQL_DRIVER_H

#include <vector>
#include "../global_define.h"
#include <pg_lib/pg_connection.h>
#include <pg_lib/pg_prepared_statement.h>


int driver(pg_connection &,
        std::vector<pg_prepared_statement> &, int, file_obj *);

int driver(pg_connection &, int, file_obj *);

#endif //POSTGRESQL_DRIVER_H