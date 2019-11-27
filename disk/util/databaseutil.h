//
// Created by ilmare on 11/27/19.
//

#ifndef DISK_DATABASEUTIL_H
#define DISK_DATABASEUTIL_H


#include "../domain/pg_database.h"

class databaseutil {
public:
    static pg_database getDatabase(string name);
};


#endif //DISK_DATABASEUTIL_H
