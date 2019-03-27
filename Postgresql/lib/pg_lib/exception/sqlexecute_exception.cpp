//
// Created by Administrator on 2019/3/26.
//

#include "sqlexecute_exception.h"
#include <iostream>

sqlexecute_exception::sqlexecute_exception(ExecStatusType type): std::exception() {
    this->type = type;
    using namespace std;
    switch (type){
        case PGRES_EMPTY_QUERY:
            cerr << "empty query string was executed" << endl;
            break;
        case PGRES_COMMAND_OK:
            cerr << "a query command that doesn't return anything was executed properly by the backend" << endl;
            break;
        case PGRES_TUPLES_OK:
            cerr << "a query command that returns tuples was executed properly by the backend, PGresult contains the result tuples" << endl;
            break;
        case PGRES_COPY_OUT:
            cerr << "Copy Out data transfer in progress" << endl;
            break;
        case PGRES_COPY_IN:
            cerr << "Copy In data transfer in progress" << endl;
            break;
        case PGRES_BAD_RESPONSE:
            cerr << "an unexpected response was recv'd from the backend" << endl;
            break;
        case PGRES_NONFATAL_ERROR:
            cerr << "notice or warning message" << endl;
            break;
        case PGRES_FATAL_ERROR:
            cerr << "query failed" << endl;
            break;
        case PGRES_COPY_BOTH:
            cerr << "Copy In/Out data transfer in progress" << endl;
            break;
        case PGRES_SINGLE_TUPLE:
            cerr << "single tuple from larger resultset" << endl;
            break;
    }
}

const char* sqlexecute_exception::what() const noexcept{
    switch (this->type){
        case PGRES_EMPTY_QUERY:
            return "empty query string was executed";
        case PGRES_COMMAND_OK:
            return "a query command that doesn't return anything was executed properly by the backend";
        case PGRES_TUPLES_OK:
            return "a query command that returns tuples was executed properly by the backend, PGresult contains the result tuples";
        case PGRES_COPY_OUT:
            return "Copy Out data transfer in progress";
        case PGRES_COPY_IN:
            return "Copy In data transfer in progress";
        case PGRES_BAD_RESPONSE:
            return "an unexpected response was recv'd from the backend";
        case PGRES_NONFATAL_ERROR:
            return "notice or warning message";
        case PGRES_FATAL_ERROR:
            return "query failed";
        case PGRES_COPY_BOTH:
            return "Copy In/Out data transfer in progress";
        default:
            return "single tuple from larger resultset";
    }
}
