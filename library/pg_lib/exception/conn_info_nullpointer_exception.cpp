//
// Created by ilmare on 2/24/19.
//

#include "conn_info_nullpointer_exception.h"
#include <iostream>

conn_info_nullpointer_exception::conn_info_nullpointer_exception(const char *connection_info): std::exception() {
    std::cerr << "There is no connection information" << std::endl;
}

conn_info_nullpointer_exception::conn_info_nullpointer_exception(): std::exception() {
    std::cerr << "There is no connection information" << std::endl;
}

const char* conn_info_nullpointer_exception::what() const noexcept {
    return "Connection Bad!";
}
