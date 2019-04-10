//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_PG_RESULTSET_H
#define POSTGRESQL_PG_RESULTSET_H

#include <libpq-fe.h>
#include <string>

class pg_resultset {
private:
    PGresult *result_set;
    int row_count;
    int column_count;
    int cursor;
public:
    pg_resultset(PGresult *res);
    pg_resultset(const pg_resultset &);
    ~pg_resultset();

    pg_resultset &operator=(const pg_resultset &);

    bool has_next();
    int get_column_count();
    int get_tuples_count();
    void show_with_elegant_format();

    std::string get_field(int idx);

    char *get_value(int idx);
    void close();
};

#endif //POSTGRESQL_PG_RESULTSET_H
