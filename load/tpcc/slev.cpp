//
// Created by Administrator on 2019/4/12.
//
#include <vector>
#include <cstring>
#include <random>
#include "../global_define.h"
#include "../lib/pg_lib/pg_resultset.h"
#include "../lib/pg_lib/pg_connection.h"
#include "./container/transaction_obj.h"
#include "../lib/pg_lib/pg_prepared_statement.h"
#include "../lib/tools/global_tools.h"

int slev(int w_id_arg, int d_id_arg,	int level_arg,
        pg_connection &con, std::vector<pg_prepared_statement> &val, file_obj *obj, int t_id) {

    pthread_t t = pthread_self();
//    std::clog << " --> Thread: [" << t << "]@"
//              << (void *)&t << ", function [slev]@"
//              << (void *)slev << std::endl;
    transaction_obj tran_obj;
    std::string tran_name = parseInt((int)t) + "@slev_" + parseInt(t_id);
    int w_id = w_id_arg;
    int d_id = d_id_arg;
    int level = level_arg;
    int d_next_o_id;
    int ol_i_id;
    try{
        con.set_auto_commit(false);
        /*
         * "SELECT d_next_o_id FROM district WHERE d_id = $1 AND d_w_id = $2",
         * const parameter_type type32[] = {int_type, int_type};
         * */
        pg_prepared_statement st = val[32];
        st.set_int(0, d_id);
        st.set_int(1, w_id);
        clock_t start = clock();
        pg_resultset res = st.execute_query();
        clock_t end = clock();
        while(res.has_next()){
            d_next_o_id = res.get_int(0);
        }
        tran_obj.add_item(transaction_item(SHARED_LOCK, "district",
                res.get_tuples_count(), start - obj->start, tran_name, end - start));
//        std::clog << " ----> Thread: [" << t << "]@"
//                  << (void *)&t << ", function [slev]@" << (void *)slev
//                  << ", pg_prepared_statement [st]@"
//                  << (void *)&st << std::endl;

        /*
         * const parameter_type type33[] = {int_type, int_type, int_type, int_type};
         * "SELECT DISTINCT ol_i_id FROM order_line WHERE ol_w_id = $1 AND ol_d_id = $2 AND ol_o_id < $3 AND ol_o_id >= ($4 - 20)",
         * */
        pg_prepared_statement st1 = val[33];

        st1.set_int(0, w_id);
        st1.set_int(1, d_id);
        st1.set_int(2, d_next_o_id);
        st1.set_int(3, d_next_o_id);
        start = clock();
        pg_resultset res1 = st1.execute_query();
        end = clock();
        while(res1.has_next()){
            ol_i_id = res1.get_int(0);
        }
        tran_obj.add_item(transaction_item(SHARED_LOCK, "order_line",
                res1.get_tuples_count(), start - obj->start, tran_name, end - start));
//        std::clog << " ----> Thread: [" << t << "]@"
//                  << (void *)&t << ", function [slev]@" << (void *)slev
//                  << ", pg_prepared_statement [st1]@"
//                  << (void *)&st1 << std::endl;
        /*
         * "SELECT count(*) FROM stock WHERE s_w_id = $1 AND s_i_id = $2 AND s_quantity < $3"
         * const parameter_type type34[] = {int_type, int_type, int_type};
         * */
        pg_prepared_statement st2 = val[34];

        st2.set_int(0, w_id);
        st2.set_int(1, ol_i_id);
        st2.set_int(2, level);
        start = clock();
        pg_resultset res2 = st2.execute_query();
        end = clock();
        tran_obj.add_item(transaction_item(SHARED_LOCK, "stock",
                res2.get_tuples_count(), start - obj->start, tran_name, end - start));
//        std::clog << " ----> Thread: [" << t << "]@"
//                  << (void *)&t << ", function [slev]@" << (void *)slev
//                  << ", pg_prepared_statement [st2]@"
//                  << (void *)&st2 << std::endl;
        con.commit();
        pthread_mutex_lock(&obj->mutex);
        for(int idx = 0; idx < tran_obj.size(); idx ++){
            transaction_item item = tran_obj[idx];
            obj->out << item.tran_name << ","
                     << item.mode << "," << item.table
                     << "," << item.row << "," << item.execute_t << ","
                     << item.t << std::endl;
        }
        pthread_mutex_unlock(&obj->mutex);
    }catch(std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}