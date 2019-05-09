//
// Created by ilmare on 2/24/19.
//

#include "conn_fail_exception.h"
#include <iostream>

conn_fail_exception::conn_fail_exception(): std::exception(){
    std::cerr << "connection failed" << std::endl;
}

conn_fail_exception::conn_fail_exception(ConnStatusType statusType): std::exception() {
    std::cerr << "connection failed, status is " << statusType << std::endl;
}

const char* conn_fail_exception::what() const noexcept{
    return "connection failed";
}
