//
// Created by Administrator on 2019/3/26.
//

#ifndef POSTGRESQL_RESULTSET_NULLPOINTER_EXCEPTION_H
#define POSTGRESQL_RESULTSET_NULLPOINTER_EXCEPTION_H

#include <exception>
#include <string>

class resultset_nullpointer_exception: public std::exception{
private:
    std::string err_msg;
public:
    resultset_nullpointer_exception(const std::string &);

    virtual const char *what() const noexcept override;
};


#endif //POSTGRESQL_RESULTSET_NULLPOINTER_EXCEPTION_H
