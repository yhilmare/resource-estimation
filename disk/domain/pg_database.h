//
// Created by ilmare on 11/21/19.
//

#ifndef DISK_PG_DATABASE_H
#define DISK_PG_DATABASE_H

#include <iostream>
#include <pg_lib/pg_resultset.h>
#include <string>
#include "../factory/objectfactory.h"

using namespace std;
class pg_database {
public:
    string datid;
    string datname;
    long blks_read;
    long blks_hit;
    long deadlocks;
    double blk_read_time;
    double blk_write_time;
public:
    pg_database();
    pg_database(string, string, long,
            long, long, double, double);

    friend ostream &operator<<(ostream &out, const pg_database &instance);
};

void *createInstance();

#endif //DISK_PG_DATABASE_H
