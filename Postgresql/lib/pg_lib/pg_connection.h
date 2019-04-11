//
// Created by ilmare on 2/24/19.
//

#ifndef POSTGRESQL_PG_CONNECTION_H
#define POSTGRESQL_PG_CONNECTION_H

#include <libpq-fe.h>
#include "pg_statement.h"
#include "pg_prepared_statement.h"
#include <iostream>

class pg_connection {
private:
    PGconn *pg_conn = NULL;
    ConnStatusType connection_status;
    char *conn_info = NULL;
    bool auto_commit = true;
public:
    pg_connection(const char *connection_info);
    pg_connection(const char *user_name,
            const char *password,
            const char *host_name,
            const char *database,
            const char *port,
            int connection_timeout=10);
    pg_connection(const pg_connection &);
    ~pg_connection();

    void set_auto_commit(bool);
    void roll_back();
    void commit();
    pg_connection &operator=(const pg_connection &);

    pg_statement create_statement();
    pg_prepared_statement prepared_statement(std::string &, const parameter_type[]);
    void close();

    friend std::ostream &operator<<(std::ostream &os, const pg_connection &con);
};

#endif //POSTGRESQL_PG_CONNECTION_H
