//
// Created by Administrator on 2019/4/11.
//
#include "../lib/pg_lib/pg_connection.h"
#include "../lib/pg_lib/pg_prepared_statement.h"
#include "../lib/pg_lib/pg_resultset.h"
#include <vector>
#include "../global_define.h"
#include <random>
#include <cstring>

#define pick_dist_info(ol_dist_info,ol_supply_w_id) \
switch(ol_supply_w_id) { \
case 1: strncpy(ol_dist_info, s_dist_01, 25); break; \
case 2: strncpy(ol_dist_info, s_dist_02, 25); break; \
case 3: strncpy(ol_dist_info, s_dist_03, 25); break; \
case 4: strncpy(ol_dist_info, s_dist_04, 25); break; \
case 5: strncpy(ol_dist_info, s_dist_05, 25); break; \
case 6: strncpy(ol_dist_info, s_dist_06, 25); break; \
case 7: strncpy(ol_dist_info, s_dist_07, 25); break; \
case 8: strncpy(ol_dist_info, s_dist_08, 25); break; \
case 9: strncpy(ol_dist_info, s_dist_09, 25); break; \
case 10: strncpy(ol_dist_info, s_dist_10, 25); break; \
}

int neword(int w_id_arg, int d_id_arg,
        int c_id_arg, int o_ol_cnt_arg, int o_all_local_arg,
        int itemid[], int supware[], int qty[], pg_connection &con,
        std::vector<pg_prepared_statement> &val) {

    pthread_t t = pthread_self();
    std::clog << " --> Thread: [" << t << "]@"
              << (void *)&t << ", function [neword]@"
              << (void *)neword << std::endl;
    int w_id = w_id_arg;
    int d_id = d_id_arg;
    int c_id = c_id_arg;
    int o_ol_cnt = o_ol_cnt_arg;
    int o_all_local = o_all_local_arg;
    float c_discount;
    char c_last[17];
    char c_credit[3];
    float w_tax;
    int d_next_o_id;
    float d_tax;
    PG::Date current;
    long seconds = current.get_million_seconds();
    extern std::default_random_engine e;
    std::uniform_int_distribution<long> d3(0L, 630720000L);
    int o_id;
    char i_name[25];
    float i_price;
    char i_data[51];
    int ol_i_id;
    int s_quantity;
    char s_data[51];
    char s_dist_01[25];
    char s_dist_02[25];
    char s_dist_03[25];
    char s_dist_04[25];
    char s_dist_05[25];
    char s_dist_06[25];
    char s_dist_07[25];
    char s_dist_08[25];
    char s_dist_09[25];
    char s_dist_10[25];
    char ol_dist_info[25];
    int ol_supply_w_id;
    float ol_amount;
    int ol_number;
    int ol_quantity;

    char iname[MAX_NUM_ITEMS][MAX_ITEM_LEN];
    char bg[MAX_NUM_ITEMS];
    float amt[MAX_NUM_ITEMS];
    float price[MAX_NUM_ITEMS];
    int stock[MAX_NUM_ITEMS];
    float total = 0.0;

    int min_num;
    int i,j,tmp,swp;
    int ol_num_seq[MAX_NUM_ITEMS];
    try{
        con.set_auto_commit(false);
        pg_prepared_statement st = val[0];
        /*
         * SELECT c_discount, c_last, c_credit, w_tax FROM customer, warehouse WHERE w_id = $1 AND c_w_id = w_id AND c_d_id = $2 AND c_id = $3
         * const parameter_type type0[] = {int_type, int_type, int_type};
         * */
        st.set_int(0, w_id);
        st.set_int(1, d_id);
        st.set_int(2, c_id);
        pg_resultset res = st.execute_query();
        while(res.has_next()){
            c_discount = res.get_float(0);
            strcpy(c_last, res.get_value(1));
            strcpy(c_credit, res.get_value(2));
            w_tax = res.get_float(3);
        }
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [neword]@" << (void *)neword
                  << ", pg_prepared_statement [st]@"
                  << (void *)&st << std::endl;

        pg_prepared_statement st1 = val[1];
        /*
         * const parameter_type type1[] = {int_type, int_type};
         * "SELECT d_next_o_id, d_tax FROM district WHERE d_id = $1 AND d_w_id = $2 FOR UPDATE",
         * */
        st1.set_int(0, d_id);
        st1.set_int(1, w_id);
        pg_resultset res1 = st1.execute_query();
        while(res1.has_next()){
            d_next_o_id = res1.get_int(0);
            d_tax = res1.get_float(1);
        }
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [neword]@" << (void *)neword
                  << ", pg_prepared_statement [st1]@"
                  << (void *)&st1 << std::endl;

        pg_prepared_statement st2 = val[2];

        /*
         * const parameter_type type2[] = {int_type, int_type, int_type};
         * "UPDATE district SET d_next_o_id = $1 + 1 WHERE d_id = $2 AND d_w_id = $3",
         * */
        st2.set_int(0, d_next_o_id);
        st2.set_int(1, d_id);
        st2.set_int(2, w_id);
        st2.execute_update();
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [neword]@" << (void *)neword
                  << ", pg_prepared_statement [st2]@"
                  << (void *)&st2 << std::endl;

        pg_prepared_statement st3 = val[3];
        /*
         * const parameter_type type3[] = {int_type, int_type, int_type, int_type, date_type, int_type, int_type};
         * "INSERT INTO orders (o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_ol_cnt, o_all_local) VALUES($1, $2, $3, $4, $5, $6, $7)",
         * */
        st3.set_int(0, o_id);
        st3.set_int(1, d_id);
        st3.set_int(2, w_id);
        st3.set_int(3, c_id);
        PG::Date date(seconds - d3(e));
        st3.set_date(4, date);
        st3.set_int(5, o_ol_cnt);
        st3.set_int(6, o_all_local);
        st3.execute_update();
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [neword]@" << (void *)neword
                  << ", pg_prepared_statement [st3]@"
                  << (void *)&st3 << std::endl;

        pg_prepared_statement st4 = val[4];
        /*
         * {int_type, int_type, int_type};
         * "INSERT INTO new_orders (no_o_id, no_d_id, no_w_id) VALUES ($1,$2,$3)",
         * */
        st4.set_int(0, o_id);
        st4.set_int(1, d_id);
        st4.set_int(2, w_id);
        st4.execute_update();
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [neword]@" << (void *)neword
                  << ", pg_prepared_statement [st4]@"
                  << (void *)&st4 << std::endl;
        for (i = 0; i < o_ol_cnt; i++) {
            ol_num_seq[i] = i;
        }
        for (i = 0; i < (o_ol_cnt - 1); i++) {
            tmp = (MAXITEMS + 1) * supware[ol_num_seq[i]] + itemid[ol_num_seq[i]];
            min_num = i;
            for ( j = i+1; j < o_ol_cnt; j++) {
                if ( (MAXITEMS + 1) * supware[ol_num_seq[j]] + itemid[ol_num_seq[j]] < tmp ){
                    tmp = (MAXITEMS + 1) * supware[ol_num_seq[j]] + itemid[ol_num_seq[j]];
                    min_num = j;
                }
            }
            if ( min_num != i ){
                swp = ol_num_seq[min_num];
                ol_num_seq[min_num] = ol_num_seq[i];
                ol_num_seq[i] = swp;
            }
        }
        for (ol_number = 1; ol_number <= o_ol_cnt; ol_number++) {
            ol_supply_w_id = supware[ol_num_seq[ol_number - 1]];
            ol_i_id = itemid[ol_num_seq[ol_number - 1]];
            ol_quantity = qty[ol_num_seq[ol_number - 1]];

            pg_prepared_statement st5 = val[5];
            /*
             * "SELECT i_price, i_name, i_data FROM item WHERE i_id = $1",
             * const parameter_type type5[] = {int_type};
             * */
            st5.set_int(0, ol_i_id);
            pg_resultset res2 = st5.execute_query();
            while(res2.has_next()){
                i_price = res2.get_float(0);
                strcpy(i_name, res2.get_value(1));
                strcpy(i_data, res2.get_value(2));
            }
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [neword]@" << (void *)neword
                      << ", pg_prepared_statement [st5]@"
                      << (void *)&st5 << std::endl;
            price[ol_num_seq[ol_number - 1]] = i_price;
            strncpy(iname[ol_num_seq[ol_number - 1]], i_name, 25);

            pg_prepared_statement st6 = val[6];
            /*
             * "SELECT s_quantity, s_data, s_dist_01, s_dist_02, s_dist_03, s_dist_04,
             * s_dist_05, s_dist_06, s_dist_07, s_dist_08, s_dist_09, s_dist_10 FROM
             * stock WHERE s_i_id = $1 AND s_w_id = $2 FOR UPDATE",
             * const parameter_type type6[] = {int_type, int_type};
             * */
            st6.set_int(0, ol_i_id);
            st6.set_int(1, ol_supply_w_id);
            pg_resultset res3 = st6.execute_query();
            while(res.has_next()){
                s_quantity = res3.get_int(0);
                strcpy(s_data, res3.get_value(1));
                strcpy(s_dist_01, res3.get_value(2));
                strcpy(s_dist_02, res3.get_value(3));
                strcpy(s_dist_03, res3.get_value(4));
                strcpy(s_dist_04, res3.get_value(5));
                strcpy(s_dist_05, res3.get_value(6));
                strcpy(s_dist_06, res3.get_value(7));
                strcpy(s_dist_07, res3.get_value(8));
                strcpy(s_dist_08, res3.get_value(9));
                strcpy(s_dist_09, res3.get_value(10));
                strcpy(s_dist_10, res3.get_value(11));
            }
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [neword]@" << (void *)neword
                      << ", pg_prepared_statement [st6]@"
                      << (void *)&st6 << std::endl;

            pick_dist_info(ol_dist_info, d_id);
            stock[ol_num_seq[ol_number - 1]] = s_quantity;

            if ((strstr(i_data, "original") != NULL) &&
                (strstr(s_data, "original") != NULL)){
                bg[ol_num_seq[ol_number - 1]] = 'B';
            } else{
                bg[ol_num_seq[ol_number - 1]] = 'G';
            }
            if (s_quantity > ol_quantity){
                s_quantity = s_quantity - ol_quantity;
            } else{
                s_quantity = s_quantity - ol_quantity + 91;
            }

            pg_prepared_statement st7 = val[7];
            /*
             * const parameter_type type7[] = {int_type, int_type, int_type};
             * "UPDATE stock SET s_quantity = $1 WHERE s_i_id = $2 AND s_w_id = $3",
             * */
            st7.set_int(0, s_quantity);
            st7.set_int(1, ol_i_id);
            st7.set_int(2, ol_supply_w_id);
            st7.execute_update();
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [neword]@" << (void *)neword
                      << ", pg_prepared_statement [st7]@"
                      << (void *)&st7 << std::endl;

            ol_amount = ol_quantity * i_price * (1 + w_tax + d_tax) * (1 - c_discount);
            amt[ol_num_seq[ol_number - 1]] = ol_amount;
            total += ol_amount;

            pg_prepared_statement st8 = val[8];
            /*
             * const parameter_type type8[] = {int_type, int_type, int_type, int_type, int_type, int_type, int_type, numeric_type, text_type};
             * "INSERT INTO order_line (ol_o_id, ol_d_id, ol_w_id, ol_number,
             * ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_dist_info)
             * VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)",
             * */
            st8.set_int(0, o_id);
            st8.set_int(1, d_id);
            st8.set_int(2, w_id);
            st8.set_int(3, ol_number);
            st8.set_int(4, ol_i_id);
            st8.set_int(5, ol_supply_w_id);
            st8.set_int(6, ol_quantity);
            st8.set_float(7, ol_amount);
            st8.set_value(8, ol_dist_info);
            st8.execute_update();
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [neword]@" << (void *)neword
                      << ", pg_prepared_statement [st8]@"
                      << (void *)&st8 << std::endl;
        }
        con.commit();
    }catch(const std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
