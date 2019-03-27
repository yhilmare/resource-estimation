//
// Created by Administrator on 2019/3/26.
//

#include "resultset_nullpointer_exception.h"
#include <iostream>

resultset_nullpointer_exception::resultset_nullpointer_exception(const std::string &msg): std::exception() {
    std::cerr << msg << std::endl;
    this->err_msg = msg;
}

const char* resultset_nullpointer_exception::what() const noexcept {
    return this->err_msg.c_str();
}
