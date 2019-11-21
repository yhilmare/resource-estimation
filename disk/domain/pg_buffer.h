//
// Created by ilmare on 11/21/19.
//

#ifndef DISK_PG_BUFFER_H
#define DISK_PG_BUFFER_H

#include <string>
#include <iostream>
#include <pg_lib/pg_resultset.h>

using namespace std;
class pg_buffer {
public:
    int bufferid;
    string relfilenode;
    string reltablespace;
    string reldatabase;
    int relforknumber;
    long relblocknumber;
    bool isdirty;
    int usagecount;
    int pinning_backends;
    double usageratio;

    pg_buffer();
    pg_buffer(pg_resultset *resultset);

    friend ostream &operator<<(ostream &out, const pg_buffer &buffer);
};


#endif //DISK_PG_BUFFER_H
