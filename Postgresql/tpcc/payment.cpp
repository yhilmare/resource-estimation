//
// Created by Administrator on 2019/4/12.
//

#include "../lib/pg_lib/pg_connection.h"
#include "../lib/pg_lib/pg_prepared_statement.h"
#include "../lib/pg_lib/pg_resultset.h"
#include <vector>
#include <cstring>
#include <random>

int payment(int w_id_arg, int d_id_arg, int byname,
        int c_w_id_arg, int c_d_id_arg, int c_id_arg,
        char c_last_arg[], float h_amount_arg, pg_connection &con,
        std::vector<pg_prepared_statement> &val) {

    pthread_t t = pthread_self();
    std::clog << " --> Thread: [" << t << "]@"
              << (void *)&t << ", function [payment]@"
              << (void *)payment << std::endl;
    int w_id = w_id_arg;
    int d_id = d_id_arg;
    int c_id = c_id_arg;
    char w_name[11];
    char w_street_1[21];
    char w_street_2[21];
    char w_city[21];
    char w_state[3];
    char w_zip[10];
    int c_d_id = c_d_id_arg;
    int c_w_id = c_w_id_arg;
    char c_first[17];
    char c_middle[3];
    char c_last[17];
    char c_street_1[21];
    char c_street_2[21];
    char c_city[21];
    char c_state[3];
    char c_zip[10];
    char c_phone[17];
    char c_since[20];
    char c_credit[4];
    float c_balance;
    char c_data[502];
    float h_amount = h_amount_arg;
    char h_data[26];
    char d_name[11];
    char d_street_1[21];
    char d_street_2[21];
    char d_city[21];
    char d_state[3];
    char d_zip[10];
    PG::Date current;
    long seconds = current.get_million_seconds();
    extern std::default_random_engine e;
    std::uniform_int_distribution<long> d3(0L, 630720000L);
    try{
        con.set_auto_commit(false);
        /*
         * const parameter_type type9[] = {numeric_type, int_type};
         * "UPDATE warehouse SET w_ytd = w_ytd + $1 WHERE w_id = $2",
         * */
        pg_prepared_statement st = val[9];
        st.set_float(0, h_amount);
        st.set_int(1, w_id);
        st.execute_update();
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [payment]@" << (void *)payment
                  << ", pg_prepared_statement [st]@"
                  << (void *)&st << std::endl;
        /*
         * "SELECT w_street_1, w_street_2, w_city, w_state, w_zip, w_name FROM warehouse WHERE w_id = $1",
         * const parameter_type type10[] = {int_type};
         * */
        pg_prepared_statement st1 = val[10];

        st1.set_int(0, w_id);
        pg_resultset res = st1.execute_query();
        while(res.has_next()){
            strcpy(w_street_1, res.get_value(0));
            strcpy(w_street_2, res.get_value(1));
            strcpy(w_city, res.get_value(2));
            strcpy(w_state, res.get_value(3));
            strcpy(w_zip, res.get_value(4));
            strcpy(w_name, res.get_value(5));
        }
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [payment]@" << (void *)payment
                  << ", pg_prepared_statement [st1]@"
                  << (void *)&st1 << std::endl;
        /*
         * const parameter_type type11[] = {numeric_type, int_type, int_type};
         * "UPDATE district SET d_ytd = d_ytd + $1 WHERE d_w_id = $2 AND d_id = $3",
         * */
        pg_prepared_statement st2 = val[11];

        st2.set_float(0, h_amount);
        st2.set_int(1, w_id);
        st2.set_int(2, d_id);
        st2.execute_update();
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [payment]@" << (void *)payment
                  << ", pg_prepared_statement [st2]@"
                  << (void *)&st2 << std::endl;
        /*
         * const parameter_type type12[] = {int_type, int_type};
         * "SELECT d_street_1, d_street_2, d_city, d_state, d_zip, d_name FROM district WHERE d_w_id = $1 AND d_id = $2",
         * */
        pg_prepared_statement st3 = val[12];

        st3.set_int(0, w_id);
        st3.set_int(1, d_id);
        pg_resultset res1 = st3.execute_query();
        while(res1.has_next()){
            strcpy(d_street_1, res1.get_value(0));
            strcpy(d_street_2, res1.get_value(1));
            strcpy(d_city, res1.get_value(2));
            strcpy(d_state, res1.get_value(3));
            strcpy(d_zip, res1.get_value(4));
            strcpy(d_name, res1.get_value(5));
        }
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [payment]@" << (void *)payment
                  << ", pg_prepared_statement [st3]@"
                  << (void *)&st3 << std::endl;
        if (byname) {
            strcpy(c_last, c_last_arg);
            /*
             * "SELECT count(c_id) FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3",
             * const parameter_type type13[] = {int_type, int_type, text_type};
             * */
            pg_prepared_statement st4 = val[13];

            st4.set_int(0, c_w_id);
            st4.set_int(1, c_d_id);
            st4.set_value(2, c_last);
            pg_resultset res2 = st4.execute_query();
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [payment]@" << (void *)payment
                      << ", pg_prepared_statement [st4]@"
                      << (void *)&st4 << std::endl;
            /*
             * const parameter_type type14[] = {int_type, int_type, text_type};
             * "SELECT c_id FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_last = $3 ORDER BY c_first",
             * */
            pg_prepared_statement st5 = val[14];

            st5.set_int(0, c_w_id);
            st5.set_int(1, c_d_id);
            st5.set_value(2, c_last);
            pg_resultset res3 = st5.execute_query();
            while(res3.has_next()){
                c_id = res3.get_int(0);
            }
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [payment]@" << (void *)payment
                      << ", pg_prepared_statement [st5]@"
                      << (void *)&st5 << std::endl;
        }
        /*
         * "SELECT c_first, c_middle, c_last, c_street_1, c_street_2, c_city, c_state,
         * c_zip, c_phone, c_credit, c_credit_lim, c_discount, c_balance, c_since
         * FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_id = $3 FOR UPDATE",
         *
         * const parameter_type type15[] = {int_type, int_type, int_type};
         * */
        pg_prepared_statement st6 = val[15];

        st6.set_int(0, c_w_id);
        st6.set_int(1, c_d_id);
        st6.set_int(2, c_id);
        pg_resultset res4 = st6.execute_query();
        while(res4.has_next()){
            strcpy(c_first, res4.get_value(0));
            strcpy(c_middle, res4.get_value(1));
            strcpy(c_last, res4.get_value(2));
            strcpy(c_street_1, res4.get_value(3));
            strcpy(c_street_2, res4.get_value(4));
            strcpy(c_city, res4.get_value(5));
            strcpy(c_state, res4.get_value(6));
            strcpy(c_zip, res4.get_value(7));
            strcpy(c_phone, res4.get_value(8));
            strcpy(c_credit, res4.get_value(9));
            c_balance = res4.get_float(12);
            strcpy(c_since, res4.get_value(13));
        }
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [payment]@" << (void *)payment
                  << ", pg_prepared_statement [st6]@"
                  << (void *)&st6 << std::endl;
        c_balance = c_balance - h_amount;
        c_credit[2] = '\0';
        if (strstr(c_credit, "BC")) {
            /*
             * const parameter_type type16[] = {int_type, int_type, int_type};
             * "SELECT c_data FROM customer WHERE c_w_id = $1 AND c_d_id = $2 AND c_id = $3",
             * */
            pg_prepared_statement st7 = val[16];

            st7.set_int(0, c_w_id);
            st7.set_int(1, c_d_id);
            st7.set_int(2, c_id);
            pg_resultset res5 = st7.execute_query();
            while(res5.has_next()){
                strcpy(c_data, res5.get_value(0));
            }
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [payment]@" << (void *)payment
                      << ", pg_prepared_statement [st7]@"
                      << (void *)&st7 << std::endl;
            /*
             * "UPDATE customer SET c_balance = $1, c_data = $2 WHERE c_w_id = $3 AND c_d_id = $4 AND c_id = $5",
             * const parameter_type type17[] = {numeric_type, text_type, int_type, int_type, int_type};
             * */
            pg_prepared_statement st8 = val[17];

            st8.set_float(0, c_balance);
            st8.set_value(1, c_data);
            st8.set_int(2, c_w_id);
            st8.set_int(3, c_d_id);
            st8.set_int(4, c_id);
            st8.execute_update();
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [payment]@" << (void *)payment
                      << ", pg_prepared_statement [st8]@"
                      << (void *)&st8 << std::endl;
        } else {
            /*
             * const parameter_type type18[] = {numeric_type, int_type, int_type, int_type};
             * "UPDATE customer SET c_balance = $1 WHERE c_w_id = $2 AND c_d_id = $3 AND c_id = $4",
             * */
            pg_prepared_statement st9 = val[18];

            st9.set_float(0, c_balance);
            st9.set_int(1, c_w_id);
            st9.set_int(2, c_d_id);
            st9.set_int(3, c_id);
            st9.execute_update();
            std::clog << " ----> Thread: [" << t << "]@"
                      << (void *)&t << ", function [payment]@" << (void *)payment
                      << ", pg_prepared_statement [st9]@"
                      << (void *)&st9 << std::endl;
        }
        strncpy(h_data, w_name, 10);
        h_data[10] = '\0';
        strncat(h_data, d_name, 10);
        h_data[20] = ' ';
        h_data[21] = ' ';
        h_data[22] = ' ';
        h_data[23] = ' ';
        h_data[24] = '\0';

        /*
         * "INSERT INTO history(h_c_d_id, h_c_w_id, h_c_id, h_d_id, h_w_id, h_date, h_amount, h_data) VALUES($1, $2, $3, $4, $5, $6, $7, $8)",
         * const parameter_type type19[] = {int_type, int_type, int_type, int_type, int_type, date_type, numeric_type, text_type};
         * */
        pg_prepared_statement st10 = val[19];

        st10.set_int(0, c_d_id);
        st10.set_int(1, c_w_id);
        st10.set_int(2, c_id);
        st10.set_int(3, d_id);
        st10.set_int(4, w_id);
        PG::Date date(seconds - d3(e));
        st10.set_date(5, date);
        st10.set_float(6, h_amount);
        st10.set_value(7, h_data);
        st10.execute_update();
        std::clog << " ----> Thread: [" << t << "]@"
                  << (void *)&t << ", function [payment]@" << (void *)payment
                  << ", pg_prepared_statement [st10]@"
                  << (void *)&st10 << std::endl;
        con.commit();
    }catch(std::exception &e){
        con.roll_back();
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}