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

int ordstat(int w_id_arg, int d_id_arg, int byname,
        int c_id_arg, char c_last_arg[], pg_connection &con,
            std::vector<pg_prepared_statement> &val, file_obj *obj, int t_id){

    pthread_t t = pthread_self();
//    std::clog << " --> Thread: [" << t << "]@"
//              << (void *)&t << ", function [ordstat]@"
//              << (void *)ordstat << std::endl;
    transaction_obj tran_obj;
    std::string tran_name = parseInt((long)t) + "@ordstat_" + parseInt(t_id);
    int w_id = w_id_arg;
    int d_id = d_id_arg;
    int c_id = c_id_arg;
    int c_d_id = d_id;
    int c_w_id = w_id;
    char c_first[17];
    char c_middle[3];
    char c_last[17];
    int o_id;
    char o_entry_d[25];
    char ol_delivery_d[25];
    try{
        con.set_auto_commit(false);
        if (byname) {
            strcpy(c_last, c_last_arg);
            /*
             * const parameter_type type20[] = {int_type, int_type, text_type};
             * "SELECT count(c_id) FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3",
             * */
            pg_prepared_statement st = val[20];
            st.set_int(0, c_w_id);
            st.set_int(1, c_d_id);
            st.set_value(2, c_last);
            clock_t start = clock();
            pg_resultset res = st.execute_query();
            clock_t end = clock();
            tran_obj.add_item(transaction_item(SHARED_LOCK, "customer",
                    res.get_tuples_count(), start - obj->start, tran_name, end - start));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [ordstat]@" << (void *)ordstat
//                      << ", pg_prepared_statement [st]@"
//                      << (void *)&st << std::endl;

            /*
             * "SELECT c_balance, c_first, c_middle, c_last FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3 ORDER BY c_first",
             * const parameter_type type21[] = {int_type, int_type, text_type};
             * */
            pg_prepared_statement st1 = val[21];
            st1.set_int(0, c_w_id);
            st1.set_int(1, c_d_id);
            st1.set_value(2, c_last);
            start = clock();
            pg_resultset res1 = st1.execute_query();
            end = clock();
            while(res1.has_next()){
                strcpy(c_first, res1.get_value(1));
                strcpy(c_middle, res1.get_value(2));
                strcpy(c_last, res1.get_value(3));
            }
            tran_obj.add_item(transaction_item(SHARED_LOCK, "customer",
                    res1.get_tuples_count(), start - obj->start, tran_name, end - start));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [ordstat]@" << (void *)ordstat
//                      << ", pg_prepared_statement [st1]@"
//                      << (void *)&st1 << std::endl;
        } else {
            /*
             * const parameter_type type22[] = {int_type, int_type, int_type};
             * "SELECT c_balance, c_first, c_middle, c_last FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_id = $3",
             * */
            pg_prepared_statement st2 = val[22];

            st2.set_int(0, c_w_id);
            st2.set_int(1, c_d_id);
            st2.set_int(2, c_id);
            clock_t start = clock();
            pg_resultset res2 = st2.execute_query();
            clock_t end = clock();
            while(res2.has_next()){
                strcpy(c_first, res2.get_value(1));
                strcpy(c_middle, res2.get_value(2));
                strcpy(c_last, res2.get_value(3));
            }
            tran_obj.add_item(transaction_item(SHARED_LOCK, "customer",
                    res2.get_tuples_count(), start - obj->start, tran_name, end - start));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [ordstat]@" << (void *)ordstat
//                      << ", pg_prepared_statement [st2]@"
//                      << (void *)&st2 << std::endl;
        }
        /*
         * "SELECT o_id, o_entry_d, COALESCE(o_carrier_id,0) FROM orders WHERE o_w_id = $1
         * AND o_d_id = $2 AND o_c_id = $3 AND o_id = (SELECT MAX(o_id) FROM orders
         * WHERE o_w_id = $4 AND o_d_id = $5 AND o_c_id = $6)",
         * const parameter_type type23[] = {int_type, int_type, int_type, int_type, int_type, int_type};
         * */
        pg_prepared_statement st3 = val[23];
        st3.set_int(0, c_w_id);
        st3.set_int(1, c_d_id);
        st3.set_int(2, c_id);
        st3.set_int(3, c_w_id);
        st3.set_int(4, c_d_id);
        st3.set_int(5, c_id);
        clock_t start = clock();
        pg_resultset res3 = st3.execute_query();
        clock_t end = clock();
        while(res3.has_next()){
            o_id = res3.get_int(0);
            strcpy(o_entry_d, res3.get_value(1));
        }
        tran_obj.add_item(transaction_item(SHARED_LOCK, "orders",
                res3.get_tuples_count(), start - obj->start, tran_name, end - start));
//        std::clog << " ----> Thread: [" << t << "]@"
//                  << (void *)&t << ", function [ordstat]@" << (void *)ordstat
//                  << ", pg_prepared_statement [st3]@"
//                  << (void *)&st3 << std::endl;
        /*
         * const parameter_type type24[] = {int_type, int_type, int_type};
         * "SELECT ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_delivery_d FROM order_line WHERE ol_w_id = $1 AND ol_d_id = $2 AND ol_o_id = $3",
         * */
        pg_prepared_statement st4 = val[24];

        st4.set_int(0, c_w_id);
        st4.set_int(1, c_d_id);
        st4.set_int(2, o_id);
        start = clock();
        pg_resultset res4 = st4.execute_query();
        end = clock();
        while(res4.has_next()){
            strcpy(ol_delivery_d, res4.get_value(4));
        }
        tran_obj.add_item(transaction_item(SHARED_LOCK, "order_line",
                res4.get_tuples_count(), start - obj->start, tran_name, end - start));
//        std::clog << " ----> Thread: [" << t << "]@"
//                  << (void *)&t << ", function [ordstat]@" << (void *)ordstat
//                  << ", pg_prepared_statement [st4]@"
//                  << (void *)&st4 << std::endl;
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