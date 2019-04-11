//
// Created by Administrator on 2019/4/9.
//

#include "driver.h"
#include <time.h>
#include <iostream>
#include "data_load.h"
#include "../tpcc/sequence.h"

int driver(pg_connection &con, std::vector<pg_prepared_statement> &val, int thread_num){
    int total_time = EXECUTE_TIME * 1000;
    clock_t start = clock();

    while((clock() - start) <= total_time){

    }
}