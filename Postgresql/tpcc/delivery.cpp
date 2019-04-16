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

//transaction_item(lock_type type, std::string table_name,
//        int line, clock_t time): mode(type),
//        table(table_name), row(line), t(time){
//    pNext = NULL;
//}

int delivery(int w_id_arg, int o_carrier_id_arg,
        pg_connection &con, std::vector<pg_prepared_statement> &val, file_obj *obj) {

    transaction_obj tran_obj;
    pthread_t t = pthread_self();
    std::clog << " --> Thread: [" << t << "]@"
              << (void *)&t << ", function [delivery]@"
              << (void *)delivery << std::endl;
    std::string tran_name = parseInt((int)t) + "@delivery";
    int w_id = w_id_arg;
    int o_carrier_id = o_carrier_id_arg;
    int d_id;
    int c_id;
    int no_o_id;
    float ol_total;
    PG::Date current;
    long seconds = current.get_million_seconds();
    try{
        con.set_auto_commit(false);
        extern std::default_random_engine e;
        std::uniform_int_distribution<long> d3(0L, 630720000L);
        for (d_id = 1; d_id <= DISTRICT_PER_WAREHOUSE; d_id++) {
            /*
             * "SELECT COALESCE(MIN(no_o_id),0) FROM new_orders WHERE no_d_id = $1 AND no_w_id = $2",
             * const parameter_type type25[] = {int_type, int_type};
             * */
            pg_prepared_statement st = val[25];
            st.set_int(0, d_id);
            st.set_int(1, w_id);
            pg_resultset res = st.execute_query();
            while(res.has_next()){
                no_o_id = res.get_int(0);
            }
            tran_obj.add_item(transaction_item(SHARED_LOCK, "new_orders",
                    res.get_tuples_count(), clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st]@"
//                      << (void *)&st << std::endl;
            if(no_o_id == 0){
                continue;
            }
            /*
             * "DELETE FROM new_orders WHERE no_o_id = $1 AND no_d_id = $2 AND no_w_id = $3",
             * const parameter_type type26[] = {int_type, int_type, int_type};
             * */
            pg_prepared_statement st1 = val[26];
            st1.set_int(0, no_o_id);
            st1.set_int(1, d_id);
            st1.set_int(2, w_id);
            int row_count = st1.execute_update();
            tran_obj.add_item(transaction_item(EXCLUSIVE_LOCK, "new_orders",
                    row_count, clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st1]@"
//                      << (void *)&st1 << std::endl;
            /*
             * const parameter_type type27[] = {int_type, int_type, int_type};
             * "SELECT o_c_id FROM orders WHERE o_id = $1 AND o_d_id = $2 AND o_w_id = $3",
             * */
            pg_prepared_statement st2 = val[27];
            st2.set_int(0, no_o_id);
            st2.set_int(1, d_id);
            st2.set_int(2, w_id);
            pg_resultset res1 = st2.execute_query();
            while(res1.has_next()){
                c_id = res1.get_int(0);
            }
            tran_obj.add_item(transaction_item(SHARED_LOCK, "orders",
                    res1.get_tuples_count(), clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st2]@"
//                      << (void *)&st2 << std::endl;
            /*
             * "UPDATE orders SET o_carrier_id = $1 WHERE o_id = $2 AND o_d_id = $3 AND o_w_id = $4",
             * const parameter_type type28[] = {int_type, int_type, int_type, int_type};
             * */
            pg_prepared_statement st3 = val[28];
            st3.set_int(0, o_carrier_id);
            st3.set_int(1, no_o_id);
            st3.set_int(2, d_id);
            st3.set_int(3, w_id);
            row_count = st3.execute_update();
            tran_obj.add_item(transaction_item(EXCLUSIVE_LOCK, "orders",
                    row_count, clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st3]@"
//                      << (void *)&st3 << std::endl;
            /*
             * const parameter_type type29[] = {date_type, int_type, int_type, int_type};
             * "UPDATE order_line SET ol_delivery_d = $1 WHERE ol_o_id = $2 AND ol_d_id = $3 AND ol_w_id = $4",
             * */
            pg_prepared_statement st4 = val[29];
            PG::Date date(seconds - d3(e));
            st4.set_date(0, date);
            st4.set_int(1, no_o_id);
            st4.set_int(2, d_id);
            st4.set_int(3, w_id);
            row_count = st4.execute_update();
            tran_obj.add_item(transaction_item(EXCLUSIVE_LOCK, "order_line",
                    row_count, clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st4]@"
//                      << (void *)&st4 << std::endl;
            /*
             * "SELECT SUM(ol_amount) FROM order_line WHERE ol_o_id = $1 AND ol_d_id = $2 AND ol_w_id = $3",
             * const parameter_type type30[] = {int_type, int_type, int_type};
             * */
            pg_prepared_statement st5 = val[30];
            st5.set_int(0, no_o_id);
            st5.set_int(1, d_id);
            st5.set_int(2, w_id);
            pg_resultset res2 = st5.execute_query();
            while(res2.has_next()){
                ol_total = res2.get_float(0);
            }
            tran_obj.add_item(transaction_item(SHARED_LOCK, "order_line",
                    res2.get_tuples_count(), clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st5]@"
//                      << (void *)&st5 << std::endl;
            /*
             * const parameter_type type31[] = {numeric_type, int_type, int_type, int_type};
             * "UPDATE customer SET c_balance = c_balance + $1 , c_delivery_cnt = c_delivery_cnt + 1 WHERE c_id = $2 AND c_d_id = $3 AND c_w_id = $4",
             * */
            pg_prepared_statement st6 = val[31];

            st6.set_float(0, ol_total);
            st6.set_int(1, c_id);
            st6.set_int(2, d_id);
            st6.set_int(3, w_id);
            row_count = st6.execute_update();
            tran_obj.add_item(transaction_item(EXCLUSIVE_LOCK, "customer",
                    row_count, clock() - obj->start, tran_name));
//            std::clog << " ----> Thread: [" << t << "]@"
//                      << (void *)&t << ", function [delivery]@" << (void *)delivery
//                      << ", pg_prepared_statement [st6]@"
//                      << (void *)&st6 << std::endl;
        }
        con.commit();
        pthread_mutex_lock(&obj->mutex);
        for(int idx = 0; idx < tran_obj.size(); idx ++){
            transaction_item item = tran_obj[idx];
            obj->out << item.tran_name << ","
                     << item.mode << "," << item.table
                     << "," << item.row << ","
                     << item.t << std::endl;
        }
        pthread_mutex_unlock(&obj->mutex);
    }catch (std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}