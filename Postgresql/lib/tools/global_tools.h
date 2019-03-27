//
// Created by Administrator on 2019/3/27.
//

#ifndef POSTGRESQL_GLOBAL_TOOLS_H
#define POSTGRESQL_GLOBAL_TOOLS_H

#include <string>
#include "date.h"
#include <unordered_map>

const std::string parseInt(int);

const std::string get_pg_date_string(const PG::Date &);

const std::string get_pg_string(const char *);

const std::unordered_map<std::string, std::string> parse_properties_file(const std::string &);

#endif //POSTGRESQL_GLOBAL_TOOLS_H
