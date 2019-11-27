//
// Created by ilmare on 11/21/19.
//

#include "pg_database.h"

pg_database::pg_database(){}

pg_database::pg_database(std::string datid, std::string datname,
        long blks_read, long blks_hit, long deadlocks,
        double blk_read_time, double blk_write_time) {
    this->datid = datid;
    this->datname = datname;
    this->blks_read = blks_read;
    this->blks_hit = blks_hit;
    this->deadlocks = deadlocks;
    this->blk_read_time = blk_read_time;
    this->blk_write_time = blk_write_time;
}

ostream &operator<<(ostream &out, const pg_database &instance){
    out << "[" << instance.datname << "@" << instance.datid << "]@" << (void *)&instance;
    return out;
}

void *createInstance(){
    pg_database *obj = new pg_database("16384", "tpcc", 1500, 1500, 1500, 500.63, 458.12);
    return obj;
}
