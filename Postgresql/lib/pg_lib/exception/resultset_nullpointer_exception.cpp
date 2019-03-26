//
// Created by Administrator on 2019/3/26.
//

#include "resultset_nullpointer_exception.h"

resultset_nullpointer_exception::resultset_nullpointer_exception(const std::string &msg) {
    this->err_msg = msg;
}

const char* resultset_nullpointer_exception::what() const noexcept {
    return this->err_msg.c_str();
}
