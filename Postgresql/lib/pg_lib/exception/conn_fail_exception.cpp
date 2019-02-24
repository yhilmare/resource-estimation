//
// Created by ilmare on 2/24/19.
//

#include "conn_fail_exception.h"
#include <iostream>

conn_fail_exception::conn_fail_exception() {
    std::cerr << "connection failed" << std::endl;
}

conn_fail_exception::conn_fail_exception(ConnStatusType statusType) {
    std::cerr << "connectioj failed, status is " << statusType << std::endl;
}

const char* conn_fail_exception::what() {
    return "connectioj failed";
}
