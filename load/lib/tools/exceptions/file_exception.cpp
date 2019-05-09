//
// Created by Administrator on 2019/3/27.
//

#include "file_exception.h"
#include <iostream>

file_exception::file_exception(const char *msg): std::exception() {
    std::cerr << msg << std::endl;
    this->err_msg = msg;
}

const char* file_exception::what() const noexcept{
    return this->err_msg;
}
