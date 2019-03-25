//
// Created by ilmare on 2/24/19.
//

#ifndef POSTGRESQL_CONN_FAIL_EXCEPTION_H
#define POSTGRESQL_CONN_FAIL_EXCEPTION_H

#include <exception>
#include <libpq-fe.h>

class conn_fail_exception: public std::exception {
public:
    conn_fail_exception(ConnStatusType statusType);
    conn_fail_exception();

    virtual const char *what() const noexcept override;
};


#endif //POSTGRESQL_CONN_FAIL_EXCEPTION_H
