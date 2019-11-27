//
// Created by ilmare on 11/21/19.
//

#ifndef DISK_BUFFERUTIL_H
#define DISK_BUFFERUTIL_H

#include <vector>
#include "../domain/pg_buffer.h"
#include "../domain/pg_database.h"

using namespace std;
class bufferutil {
private:
    pg_database &database;
public:
    bufferutil(pg_database &);

    vector<pg_buffer> getBuffers();

};


#endif //DISK_BUFFERUTIL_H
