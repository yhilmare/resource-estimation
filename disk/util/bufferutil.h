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
public:
    static vector<pg_buffer> getBuffers(const pg_database &database);
};


#endif //DISK_BUFFERUTIL_H
