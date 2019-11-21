//
// Created by ilmare on 11/21/19.
//

#ifndef DISK_DATABASSUTIL_H
#define DISK_DATABASSUTIL_H

#include "../domain/pg_database.h"

class databassutil {
public:
    static pg_database getDatabase(string name);
};


#endif //DISK_DATABASSUTIL_H
