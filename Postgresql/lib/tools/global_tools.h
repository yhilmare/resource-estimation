//
// Created by Administrator on 2019/3/27.
//

#ifndef POSTGRESQL_GLOBAL_TOOLS_H
#define POSTGRESQL_GLOBAL_TOOLS_H

#include <string>
#include "date.h"

const std::string parseInt(int);

const std::string get_pg_date_string(const PG_DATE::Date &);

const std::string get_pg_string(const char *);

#endif //POSTGRESQL_GLOBAL_TOOLS_H
