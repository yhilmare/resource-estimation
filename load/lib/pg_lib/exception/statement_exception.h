//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_STATEMENT_EXCEPTION_H
#define POSTGRESQL_STATEMENT_EXCEPTION_H

#include <exception>

class statement_exception: public std::exception {
private:
    const char *err_msg;
public:
    statement_exception(const char *);

    virtual const char *what() const noexcept override;

};


#endif //POSTGRESQL_STATEMENT_EXCEPTION_H
