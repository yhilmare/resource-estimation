//
// Created by Administrator on 2019/4/15.
//

#include "transaction_obj.h"


transaction_obj::transaction_obj() {
    this->pHead = new transaction_item;
    this->cursor = pHead;
    this->cursor->pNext = NULL;
}

void transaction_obj::add_item(const transaction_item &item) {
    transaction_item *item_tmp = new transaction_item;
    item_tmp->table = item.table;
    item_tmp->t = item.t;
    item_tmp->mode = item.mode;
    item_tmp->row = item.row;
    this->cursor->pNext = item_tmp;
    this->cursor = item_tmp;
    this->cursor->pNext = NULL;
    this->count ++;
}

transaction_obj::~transaction_obj() {
    transaction_item *thenext = NULL;
    for (transaction_item *p = this->pHead; p; p = thenext){
        thenext = p->pNext;
        delete p;
    }
}

void transaction_obj::foreach(void (*f)(transaction_item *)) {
    for(transaction_item *p = this->pHead->pNext; p; p = p->pNext){
        f(p);
    }
}

transaction_item& transaction_obj::operator[](int idx) {
    if (idx >= count || idx < 0){
        throw std::exception();
    }
    int i = -1;
    for(transaction_item *p = this->pHead; p; p = p->pNext, i ++){
        if (i == idx){
            return *p;
        }
    }
    throw std::exception();
}

int transaction_obj::size() {
    return this->count;
}