//
// Created by Administrator on 2019/3/27.
//

#ifndef POSTGRESQL_FILE_EXCEPTION_H
#define POSTGRESQL_FILE_EXCEPTION_H

#include <exception>
#include <string>

class file_exception: public std::exception {
private:
    const char *err_msg;
public:
    file_exception(const char *);

    virtual const char *what() const noexcept override;

};


#endif //POSTGRESQL_FILE_EXCEPTION_H
