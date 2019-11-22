//
// Created by ilmare on 11/21/19.
//

#include "pg_buffer.h"

pg_buffer::pg_buffer() {}

pg_buffer::pg_buffer(pg_resultset *resultset) {
    if (resultset == NULL){
        throw exception();
    }
    this->bufferid = resultset->get_int(0);
    this->relfilenode = resultset->get_string(1);
    this->reltablespace = resultset->get_string(2);
    this->reldatabase = resultset->get_string(3);
    this->relforknumber = resultset->get_int(4);
    this->relblocknumber = resultset->get_int(5);
    if (resultset->get_string(6) == "true"){
        this->isdirty = true;
    }else{
        this->isdirty = false;
    }
    this->usagecount = resultset->get_int(7);
    this->pinning_backends = resultset->get_int(8);
}

ostream &operator<<(ostream &out, const pg_buffer &buffer){
    out << "[" << buffer.bufferid << " " << buffer.relfilenode
        << " " << buffer.reltablespace << " "
        << buffer.reldatabase << " " << buffer.relforknumber
        << " " << buffer.relblocknumber << " " << buffer.isdirty
        << " " << buffer.usagecount << " " << buffer.pinning_backends
        << " " << buffer.usageratio << "]@" << (int *)&buffer;
    return out;
}
