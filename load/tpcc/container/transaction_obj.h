//
// Created by Administrator on 2019/4/15.
//

#ifndef POSTGRESQL_TRANSACTION_OBJ_H
#define POSTGRESQL_TRANSACTION_OBJ_H

#include <iostream>
#include <time.h>
#include <string>

enum lock_type{
    EXCLUSIVE_LOCK=0,
    SHARED_LOCK
};

struct transaction_item{
    std::string tran_name;
    lock_type mode;
    std::string table;
    int row;
    long t;
    long execute_t;
    transaction_item *pNext;
    transaction_item(lock_type type, std::string table_name,
            int line, long time, std::string name, long t1): mode(type),
            table(table_name), row(line), t(time), tran_name(name), execute_t(t1){
        pNext = NULL;
    }
    transaction_item(){
        pNext = NULL;
    }
    friend std::ostream &operator<<(std::ostream &os, const transaction_item &item){
        os << "[transaction_item]@" << &item;
        return os;
    }
};

class transaction_obj {
private:
    transaction_item *pHead;
    transaction_item *cursor;
    int count = 0;
public:
    transaction_obj();
    ~transaction_obj();

    int size();
    void foreach(void (*f)(transaction_item *));

    void add_item(const transaction_item &item);

    transaction_item &operator[](int idx);
};

#endif //POSTGRESQL_TRANSACTION_OBJ_H
