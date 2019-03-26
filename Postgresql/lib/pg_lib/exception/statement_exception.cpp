//
// Created by Administrator on 2019/3/26.
//

#include "statement_exception.h"
#include <iostream>

statement_exception::statement_exception(char *msg) {
    std::cerr << msg << std::endl;
    this->err_msg = msg;
}
const char* statement_exception::what() const noexcept{
    return this->err_msg;
}
