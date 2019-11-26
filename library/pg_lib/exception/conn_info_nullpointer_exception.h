//
// Created by ilmare on 2/24/19.
//

#ifndef POSTGRESQL_CONN_INFO_NULLPOINTER_EXCEPTION_H
#define POSTGRESQL_CONN_INFO_NULLPOINTER_EXCEPTION_H

#include <exception>


class conn_info_nullpointer_exception: public std::exception{
public:
    conn_info_nullpointer_exception(const char *connection_info);
    conn_info_nullpointer_exception();
    virtual const char *what() const noexcept override;
};

#endif //POSTGRESQL_CONN_INFO_NULLPOINTER_EXCEPTION_H
