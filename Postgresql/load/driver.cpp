//
// Created by Administrator on 2019/4/9.
//

#include "driver.h"
#include <time.h>
#include <iostream>
#include "data_load.h"
#include "../tpcc/sequence.h"
#include <random>
#include "../global_define.h"

int num_node = 0;
extern int num_ware;

int neword(int t_num, int w_id_arg, int d_id_arg,
           int c_id_arg, int o_ol_cnt_arg, int o_all_local_arg,
           int itemid[], int supware[], int qty[], pg_connection &con,
           std::vector<pg_prepared_statement> &val);

int payment(int t_num, int w_id_arg, int d_id_arg, int byname,
            int c_w_id_arg, int c_d_id_arg, int c_id_arg,
            char c_last_arg[], float h_amount_arg, pg_connection &con,
            std::vector<pg_prepared_statement> &val);

int ordstat(int t_num, int w_id_arg, int d_id_arg, int byname,
            int c_id_arg, char c_last_arg[], pg_connection &con,
            std::vector<pg_prepared_statement> &val);

int delivery(int t_num, int w_id_arg, int o_carrier_id_arg,
             pg_connection &con, std::vector<pg_prepared_statement> &val);


int do_neword (pg_connection &con,
        std::vector<pg_prepared_statement> &val, int t_num);

int do_payment(pg_connection &con,
               std::vector<pg_prepared_statement> &val, int t_num);

int do_ordstat (pg_connection &con,
                std::vector<pg_prepared_statement> &val, int t_num);


int driver(pg_connection &con, std::vector<pg_prepared_statement> &val, int thread_num){
    int total_time = EXECUTE_TIME * 1000;
    clock_t start = clock();

    while((clock() - start) <= total_time){

    }

    return 0;
}

int other_ware (int home_ware) {
    int tmp;
    if (num_ware == 1){
        return home_ware;
    }
    extern std::default_random_engine e;
    std::uniform_int_distribution<int> d(1, num_ware);
    while ((tmp = d(e)) == home_ware);
    return tmp;
}

int do_neword (pg_connection &con, std::vector<pg_prepared_statement> &val, int t_num) {
    extern std::default_random_engine e;
    int num_conn = t_num;
    int c_num;
    int i, ret;
    int  w_id, d_id, c_id, ol_cnt;
    int  all_local = 1;
    int  notfound = MAXITEMS + 1;
    int rbk;
    int  itemid[MAX_NUM_ITEMS];
    int  supware[MAX_NUM_ITEMS];
    int  qty[MAX_NUM_ITEMS];

    if(num_node == 0){
        std::uniform_int_distribution<int> d(1, num_ware);
        w_id = d(e);
    }else{
        c_num = ((num_node * t_num) / num_conn); /* drop moduls */
        std::uniform_int_distribution<int> d(1 + (num_ware * c_num) / num_node,
                (num_ware * (c_num + 1)) / num_node);
        w_id = d(e);
    }

    std::uniform_int_distribution<int> d(1, DISTRICT_PER_WAREHOUSE);
    d_id = d(e);
    c_id = RANDOM_GEN::NURand(1023, 1, CUSTOMER_PER_DISTRICT);

    std::uniform_int_distribution<int> d1(5, 15);
    ol_cnt = d1(e);
    std::uniform_int_distribution<int> d2(1, 100);
    rbk = d2(e);

    std::uniform_int_distribution<int> d3(1, 10);
    for (i = 0; i < ol_cnt; i++) {
        itemid[i] = RANDOM_GEN::NURand(8191, 1, MAXITEMS);
        if ((i == ol_cnt - 1) && (rbk == 1)) {
            itemid[i] = notfound;
        }
        if (d2(e) != 1) {
            supware[i] = w_id;
        }
        else {
            supware[i] = other_ware(w_id);
            all_local = 0;
        }
        qty[i] = d3(e);
    }

    for (i = 0; i < MAX_RETRY; i++) {
        ret = neword(t_num, w_id, d_id, c_id, ol_cnt,
                all_local, itemid, supware, qty, con, val);
        if(!ret){
            break;
        }
    }
    return 0;
}


int do_payment(pg_connection &con,
        std::vector<pg_prepared_statement> &val, int t_num) {
    extern std::default_random_engine e;
    int c_num;
    int num_conn = t_num;
    int byname, i, ret;
    int  w_id, d_id, c_w_id, c_d_id, c_id, h_amount;
    char c_last[17];

    if(num_node == 0){
        std::uniform_int_distribution<int> d(1, num_ware);
        w_id = d(e);
    }else{
        c_num = ((num_node * t_num) / num_conn); /* drop moduls */
        std::uniform_int_distribution<int> d(1 + (num_ware * c_num) / num_node,
                                             (num_ware * (c_num + 1)) / num_node);
        w_id = d(e);
    }
    std::uniform_int_distribution<int> d(1, DISTRICT_PER_WAREHOUSE);
    d_id = d(e);
    c_id = RANDOM_GEN::NURand(1023, 1, CUSTOMER_PER_DISTRICT);
    RANDOM_GEN::Lastname(RANDOM_GEN::NURand(255, 0, 999), c_last);

    std::uniform_int_distribution<int> d1(1, 5000);
    h_amount = d1(e);

    std::uniform_int_distribution<int> d2(1, 100);
    if (d2(e) <= 60) {
        byname = 1; /* select by last name */
    }else{
        byname = 0; /* select by customer id */
    }
    if (d2(e) <= 85) {
        c_w_id = w_id;
        c_d_id = d_id;
    }else{
        c_w_id = other_ware(w_id);
        c_d_id = d(e);
    }

    for (i = 0; i < MAX_RETRY; i++) {
        ret = payment(t_num, w_id, d_id, byname, c_w_id, c_d_id,
                c_id, c_last, h_amount, con, val);
        if(!ret){
            break;
        }
    }
    return 0;
}

int do_ordstat (pg_connection &con,
                       std::vector<pg_prepared_statement> &val, int t_num) {
    extern std::default_random_engine e;
    int num_conn = t_num;
    int c_num;
    int byname,i,ret;
    int w_id, d_id, c_id;
    char c_last[16];

    if(num_node == 0){
        std::uniform_int_distribution<int> d(1, num_ware);
        w_id = d(e);
    }else{
        c_num = ((num_node * t_num) / num_conn); /* drop moduls */
        std::uniform_int_distribution<int> d(1 + (num_ware * c_num) / num_node,
                                             (num_ware * (c_num + 1)) / num_node);
        w_id = d(e);
    }
    std::uniform_int_distribution<int> d(1, DISTRICT_PER_WAREHOUSE);
    d_id = d(e);
    c_id = RANDOM_GEN::NURand(1023, 1, CUSTOMER_PER_DISTRICT);
    RANDOM_GEN::Lastname(RANDOM_GEN::NURand(255, 0, 999), c_last);

    std::uniform_int_distribution<int> d1(1, 100);
    if (d1(e) <= 60) {
        byname = 1; /* select by last name */
    }else{
        byname = 0; /* select by customer id */
    }
    for (i = 0; i < MAX_RETRY; i++) {
        ret = ordstat(t_num, w_id, d_id, byname, c_id, c_last, con, val);
        if(!ret){
            break;
        }
    }
    return 0;
}


int do_delivery(pg_connection &con,
        std::vector<pg_prepared_statement> &val,int t_num) {
    int num_conn = t_num;
    int c_num;
    int i,ret;
    clock_t clk1,clk2;
    double rt;
    struct timespec tbuf1;
    struct timespec tbuf2;
    int  w_id, o_carrier_id;

    if(num_node==0){
        w_id = RandomNumber(1, num_ware);
    }else{
        c_num = ((num_node * t_num)/num_conn); /* drop moduls */
        w_id = RandomNumber(1 + (num_ware * c_num)/num_node,
                            (num_ware * (c_num + 1))/num_node);
    }
    o_carrier_id = RandomNumber(1, 10);

    clk1 = clock_gettime(CLOCK_MONOTONIC, &tbuf1 );
    for (i = 0; i < MAX_RETRY; i++) {
        ret = delivery(t_num, w_id, o_carrier_id);
        clk2 = clock_gettime(CLOCK_MONOTONIC, &tbuf2 );

        if(ret){

            rt = (double)(tbuf2.tv_sec * 1000.0 + tbuf2.tv_nsec/1000000.0-tbuf1.tv_sec * 1000.0 - tbuf1.tv_nsec/1000000.0);
            if(rt > max_rt[3])
                max_rt[3]=rt;
            total_rt[3] += rt;
            hist_inc(3, rt );
            if(counting_on){
                if( rt < rt_limit[3]){
                    success[3]++;
                    success2[3][t_num]++;
                }else{
                    late[3]++;
                    late2[3][t_num]++;
                }
            }

            return (1); /* end */
        }else{

            if(counting_on){
                retry[3]++;
                retry2[3][t_num]++;
            }

        }
    }

    if(counting_on){
        retry[3]--;
        retry2[3][t_num]--;
        failure[3]++;
        failure2[3][t_num]++;
    }

    return (0);

}