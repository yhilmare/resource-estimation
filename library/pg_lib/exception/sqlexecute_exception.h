//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_SQLEXECUTE_EXCEPTION_H
#define POSTGRESQL_SQLEXECUTE_EXCEPTION_H

#include <exception>
#include <libpq-fe.h>

class sqlexecute_exception: public std::exception {
private:
    ExecStatusType type;
public:
    sqlexecute_exception(ExecStatusType type);

    virtual const char *what() const noexcept override;

};


#endif //POSTGRESQL_SQLEXECUTE_EXCEPTION_H
