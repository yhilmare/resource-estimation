//
// Created by ilmare on 2/24/19.
//

#ifndef POSTGRESQL_PG_CONNECTION_H
#define POSTGRESQL_PG_CONNECTION_H

#include <libpq-fe.h>

class pg_connection {
private:
    PGconn *pg_conn = NULL;
    ConnStatusType connection_status;
    char *conn_info;
public:
    pg_connection(char *connection_info);
    pg_connection(const char *user_name,
            const char *password,
            const char *host_name,
            const char *database,
            const char *port,
            int connection_timeout);
    ~pg_connection();
    void close();
};

#endif //POSTGRESQL_PG_CONNECTION_H
