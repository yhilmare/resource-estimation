//
// Created by Administrator on 2019/3/29.
//

#include "data_load.h"
#include <iostream>
#include <cstring>
#include <random>
#include <fstream>
#include <unistd.h>
#include <unordered_map>
#include "../global_define.h"
#include "../lib/tools/global_tools.h"
#include "../lib/pg_lib/pg_connection.h"
#include "../lib/pg_lib/pg_prepared_statement.h"

namespace RANDOM_GEN {
    std::default_random_engine e;
    int NURand (unsigned A, unsigned x, unsigned y) {
        static int first = 1;
        unsigned C, C_255, C_1023, C_8191;
        extern std::default_random_engine e;
        std::uniform_int_distribution<unsigned> d(0, 255);
        std::uniform_int_distribution<unsigned> d1(0, 1023);
        std::uniform_int_distribution<unsigned> d2(0, 8191);
        if (first) {
            C_255 = d(e);
            C_1023 = d1(e);
            C_8191 = d2(e);
            first = 0;
        }
        switch (A) {
            case 255:
                C = C_255;
                break;
            case 1023:
                C = C_1023;
                break;
            case 8191:
                C = C_8191;
                break;
            default:
                throw std::exception();
        }
        std::uniform_int_distribution<unsigned> d3(0, A);
        std::uniform_int_distribution<unsigned> d4(x, y);
        return (int)(((d3(e) | d4(e)) + C) % (y - x + 1)) + x;
    }
    void Lastname(int num, char *name) {
        const static char *n[] = {"BAR", "OUGHT", "ABLE", "PRI", "PRES",
             "ESE", "ANTI", "CALLY", "ATION", "EING"};
        strcpy(name,n[num / 100]);
        strcat(name,n[(num / 10) % 10]);
        strcat(name,n[num % 10]);
    }

    int make_alpha_string (int x, int y, char str[]){
        const char *alphanum = "0123456789"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz";
        int arrmax = 61;  /* index of last array element */
        int i, len;
        std::uniform_int_distribution<int> d(0, arrmax);
        std::uniform_int_distribution<int> d1(x, y);
        len = d1(e);

        for (i = 0; i < len; i++){
            str[i] = alphanum[d(e)];
        }
        return len;
    }
    int make_number_string(int x, int y, char str[]) {
        const char *numeric = "0123456789";
        int arrmax = 9;
        register int i, len;
        std::uniform_int_distribution<int> d(0, arrmax);
        std::uniform_int_distribution<int> d1(x, y);
        len = d1(e);
        for (i = 0; i < len; i++) {
            str[i] = numeric[d(e)];
        }
        return len;
    }

    void make_address(char *w_street_1, char *w_street_2,
                      char *w_city, char *w_state, char *w_zip) {
        using namespace std;
        const char *streets[] = {"Fulton Street", "Lexington Avenue", "Sunset Strip",
                                 "Olvera Street", "Cesar Chavez Avenue", "Macy Street",
                                 "Alameda Steet", "Madison avenue", "David Street",
                                 "Marry Avenue", "Download Street", "FullShine Street",
                                 "SunRise Street", "Tang Street", "WhiteHouse Avenue"};
        int street_length = 15;
        uniform_int_distribution<int> street_d(0, street_length - 1);

        const char *citys[] = {"Cheyenne", "Chicago", "Cincinati", "Cleveland(OH)",
                               "Columbia(SC)", "Columbus(OH)", "Concord(NH)", "Dallas",
                               "Denver", "Des Moines", "Detroit", "Dover(DE)",
                               "Frankfort(KY)", "Harrisburg", "Hartford(AL)", "Hawaii",
                               "Helena(MT)", "Houston", "Indianapolis", "Jackson(MS)",
                               "Providence(RI)", "Raleigh", "Richmond(VA)", "Sacramento",
                               "Salem(OR)", "Salt Lake City", "San Francisco", "Santa Fe(NM)",
                               "Seattle", "Springfield(IL)", "St Louis", "St Paul",
                               "Tallahassee", "Topeka", "Trenton(NJ)", "Washington D.C.",
                               "Alaska", "Albany(NY)", "Annapolis", "Atlanta(GA)",
                               "Augusta(ME)", "Austin(TX)", "Baton Rouge", "Bismarck",
                               "Boise", "Boston", "Carson City", "Charleston(WV)",
                               "Jefferson City(MO)", "Kansas City(KS)", "Lansing(MI)", "Lincoln(NE)",
                               "Little Rock", "Los Angeles", "Madison(WI)", "Memphis",
                               "Miami(FL)", "Minneapolis", "Montgomery(AL)", "Montpelier(VT)",
                               "Morristown(NJ)", "Nashville", "New Orleans", "New York",
                               "Oklahoma City", "Olympia", "Philadelphia(PA)", "Phoenix",
                               "Portland(OR)"};
        int city_length = 69;
        uniform_int_distribution<int> city_d(0, city_length - 1);

        const char *states[] = {"AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE",
                                "FL", "GA", "HI", "ID", "IL", "IN", "IA", "KS",
                                "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS",
                                "MO", "MT", "NE", "NV", "NH", "NJ", "NM", "NY",
                                "NC", "ND", "OH", "OK", "OR", "PA", "RI", "SC",
                                "SD", "TN", "TX", "UT", "VT", "VA", "WA", "WV",
                                "WI", "WY"};
        int state_length = 50;
        uniform_int_distribution<int> state_d(0, state_length - 1);

        strcpy(w_street_1, streets[street_d(e)]);
        strcpy(w_street_2, streets[street_d(e)]);
        strcpy(w_city, citys[city_d(e)]);
        strcpy(w_state, states[state_d(e)]);
        w_zip[make_number_string(9, 9, w_zip)] = 0;
    }
}

void load_item(std::unordered_map<std::string, std::string> &config){
    using namespace std;
    clog << "Loading Table: item, Max Item: " << STOCK_PER_WAREHOUSE << endl;
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];
    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), database.c_str(), port.c_str());
    try{
        con.set_auto_commit(false);
        parameter_type types[] = {int_type, int_type, text_type, numeric_type, text_type};
        string sql = "insert into item(i_id,i_im_id,i_name,i_price,i_data) values($1,$2,$3,$4,$5)";
        pg_prepared_statement st = con.prepared_statement(sql, types);
        int i_id;
        int i_im_id;
        char i_name[25];
        float i_price;
        char i_data[51];
        int idatasiz;
        int orig[STOCK_PER_WAREHOUSE + 1];
        int pos;
        std::uniform_int_distribution<int> d(0, STOCK_PER_WAREHOUSE);
        extern std::default_random_engine e;
        for (int i = 0; i < STOCK_PER_WAREHOUSE / 10; i++){
            orig[i] = 0;
        }
        for (int i = 0; i < STOCK_PER_WAREHOUSE / 10; i++) {
            do {
                pos = d(e);
            } while (orig[pos]);
            orig[pos] = 1;
        }
        std::uniform_int_distribution<int> d1(1, 10000);
        std::uniform_int_distribution<int> d2(100, 10000);

        for (i_id = 1; i_id <= STOCK_PER_WAREHOUSE; i_id++) {
            i_im_id = d1(e);
            i_name[RANDOM_GEN::make_alpha_string(14, 24, i_name)] = 0;
            i_price = (d2(e)) / 100.0;
            idatasiz = RANDOM_GEN::make_alpha_string(26, 50, i_data);
            i_data[idatasiz] = 0;
            if (orig[i_id]) {
                std::uniform_int_distribution<int> d2(0, idatasiz - 8);
                pos = d2(e);
                i_data[pos] = 'o';
                i_data[pos + 1] = 'r';
                i_data[pos + 2] = 'i';
                i_data[pos + 3] = 'g';
                i_data[pos + 4] = 'i';
                i_data[pos + 5] = 'n';
                i_data[pos + 6] = 'a';
                i_data[pos + 7] = 'l';
            }

            st.set_int(0, i_id);
            st.set_int(1, i_im_id);
            st.set_value(2, i_name);
            st.set_float(3, i_price);
            st.set_value(4, i_data);
            st.execute_update();
        }
        con.commit();
    }catch(exception &e){
        cout << e.what() << endl;
        con.roll_back();
    }
}

void load_stock(int w_id,
                pg_connection &con, int max_num);

void load_district(int w_id, pg_connection &con, int DIST_PER_WARE);

void load_warehouse(std::unordered_map<std::string, std::string> &config,
        int max_num){
    using namespace std;
    clog << "Loading Table: warehouse, Max Item: " << max_num << endl;
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];
    try{
        pg_connection con(user.c_str(), password.c_str(),
                host.c_str(), database.c_str(), port.c_str());
        parameter_type types[] = {int_type, text_type, text_type, text_type,
                                  text_type, text_type, text_type, numeric_type,
                                  numeric_type};
        string sql = "insert into warehouse(w_id,w_name,w_street_1,w_street_2,w_city,w_state,w_zip,w_tax,w_ytd) values($1,$2,$3,$4,$5,$6,$7,$8,$9);";
        pg_prepared_statement st = con.prepared_statement(sql, types);
        extern default_random_engine e;
        uniform_real_distribution<double> d(10L, 20L);
        char w_name[11];
        char w_street_1[21];
        char w_street_2[21];
        char w_city[21];
        char w_state[3];
        char w_zip[10];
        for (int i = 1; i <= max_num; i ++){
            RANDOM_GEN::make_address(w_street_1, w_street_2, w_city, w_state, w_zip);
            w_name[RANDOM_GEN::make_alpha_string(6, 10, w_name)] = 0;

            st.set_int(0, i);
            st.set_value(1, w_name);
            st.set_value(2, w_street_1);
            st.set_value(3, w_street_2);
            st.set_value(4, w_city);
            st.set_value(5, w_state);
            st.set_value(6, w_zip);
            st.set_float(7, d(e) / 100.0);
            st.set_float(8, 30000.0);
            st.execute_update();
            load_stock(i, con, STOCK_PER_WAREHOUSE);
            load_district(i, con, DISTRICT_PER_WAREHOUSE);
        }
    }catch (exception &e){
        cout << e.what() << endl;
    }
}

void load_customer(int w_id, int d_id,
                   pg_connection &con, int CUST_PER_DIST);
void load_order(int w_id, int d_id, pg_connection &con, int ORD_PER_DIST);

void load_district(int w_id, pg_connection &con, int DIST_PER_WARE){
    std::clog << "--> Loading Table: district, Max Item: " << DIST_PER_WARE << std::endl;
    try{
        int d_id;
        int d_w_id;

        char d_name[11];
        char d_street_1[21];
        char d_street_2[21];
        char d_city[21];
        char d_state[3];
        char d_zip[10];

        float d_tax;
        float d_ytd;
        int d_next_o_id;

        d_w_id = w_id;
        d_ytd = 30000.0;
        d_next_o_id = 4200L;
        extern std::default_random_engine e;
        std::uniform_real_distribution<float> d(10, 20);

        parameter_type types[] = {int_type, int_type, text_type, text_type,
                                  text_type, text_type, text_type, text_type,
                                  numeric_type, numeric_type, int_type};
        std::string sql = "insert into district(d_id,d_w_id,d_name,d_street_1,d_street_2,d_city,d_state,d_zip,d_tax,d_ytd,d_next_o_id) "
                          "values($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11)";
        pg_prepared_statement st = con.prepared_statement(sql, types);
        for (d_id = 1; d_id <= DIST_PER_WARE; d_id++) {

            d_name[RANDOM_GEN::make_alpha_string(6L, 10L, d_name)] = 0;
            RANDOM_GEN::make_address(d_street_1, d_street_2, d_city, d_state, d_zip);

            d_tax = d(e) / 100.0;
            st.set_int(0, d_id);
            st.set_int(1, d_w_id);
            st.set_value(2, d_name);
            st.set_value(3, d_street_1);
            st.set_value(4, d_street_2);
            st.set_value(5, d_city);
            st.set_value(6, d_state);
            st.set_value(7, d_zip);
            st.set_float(8, d_tax);
            st.set_float(9, d_ytd);
            st.set_int(10, d_next_o_id);
            st.execute_update();
            load_customer(w_id, d_id, con, CUSTOMER_PER_DISTRICT);
            load_order(w_id, d_id, con, ORDER_PER_DISTRICT);
        }
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void load_order(int w_id, int d_id, pg_connection &con, int ORD_PER_DIST){
    std::clog << "-->--> Loading Table: order, Max Item: " << ORD_PER_DIST << std::endl;
    std::clog << "-->--> Loading Table: new_order, Max Item: " << ORD_PER_DIST << std::endl;
    std::clog << "-->--> Loading Table: order_line, Max Item: " << ORD_PER_DIST << std::endl;
    try{
        int o_id;
        int o_c_id;
        int o_d_id;
        int o_w_id;
        int o_carrier_id;
        int o_ol_cnt;
        int ol;
        int ol_i_id;
        int ol_supply_w_id;
        int ol_quantity;
        float ol_amount;
        char ol_dist_info[25];
        float  tmp_float;

        PG::Date o_entry_d;
        long seconds = o_entry_d.get_million_seconds();

        o_d_id = d_id;
        o_w_id = w_id;

        extern std::default_random_engine e;
        std::uniform_int_distribution<int> d(1, CUSTOMER_PER_DISTRICT);
        std::uniform_int_distribution<int> d1(1, 10);
        std::uniform_int_distribution<int> d2(5, 15);

        parameter_type order_types[] = {int_type, int_type, int_type, int_type, date_type, int_type, int_type, int_type};
        std::string order_sql = "insert into orders(o_id,o_d_id,o_w_id,o_c_id,"
                                "o_entry_d,o_carrier_id,o_ol_cnt,o_all_local) values($1,$2,$3,$4,$5,$6,$7,$8)";
        pg_prepared_statement order_st = con.prepared_statement(order_sql, order_types);

        std::uniform_int_distribution<long> d3(0L, 630720000L);

        parameter_type n_o_types[] = {int_type, int_type, int_type};
        std::string n_o_sql = "insert into new_orders(no_o_id,no_d_id,no_w_id) values($1,$2,$3)";
        pg_prepared_statement n_o_st = con.prepared_statement(n_o_sql, n_o_types);

        std::uniform_int_distribution<long> d4(1, STOCK_PER_WAREHOUSE);
        std::uniform_int_distribution<long> d5(10, 10000);

        parameter_type ol_types[] = {int_type, int_type, int_type, int_type,
                                     int_type, int_type, date_type, int_type,
                                     numeric_type, text_type};
        std::string ol_sql = "insert into order_line(ol_o_id,ol_d_id,ol_w_id,ol_number,ol_i_id,ol_supply_w_id,"
                             "ol_delivery_d,ol_quantity,ol_amount,ol_dist_info) values($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)";
        pg_prepared_statement ol_st = con.prepared_statement(ol_sql, ol_types);

        for (o_id = 1; o_id <= ORD_PER_DIST; o_id++) {
            o_c_id = d(e);
            o_carrier_id = d1(e);
            o_ol_cnt = d2(e);
            PG::Date date(seconds - d3(e));
            if (o_id > 2100) {
                order_st.set_int(0, o_id);
                order_st.set_int(1, o_d_id);
                order_st.set_int(2, o_w_id);
                order_st.set_int(3, o_c_id);
                order_st.set_date(4, date);
                order_st.set_int(5, o_carrier_id);
                order_st.set_int(6, o_ol_cnt);
                order_st.set_int(7, 5000);
                order_st.execute_update();

                n_o_st.set_int(0, o_id);
                n_o_st.set_int(1, o_d_id);
                n_o_st.set_int(2, o_w_id);
                n_o_st.execute_update();
            } else {
                order_st.set_int(0, o_id);
                order_st.set_int(1, o_d_id);
                order_st.set_int(2, o_w_id);
                order_st.set_int(3, o_c_id);
                order_st.set_date(4, date);
                order_st.set_int(5, o_carrier_id);
                order_st.set_int(6, o_ol_cnt);
                order_st.set_int(7, 5000);
                order_st.execute_update();
            }
            for (ol = 1; ol <= o_ol_cnt; ol++) {
                /* Generate Order Line Data */
                ol_i_id = d4(e);
                ol_supply_w_id = o_w_id;
                ol_quantity = 5;
                ol_amount = 0.0;

                ol_dist_info[RANDOM_GEN::make_alpha_string(24, 24, ol_dist_info)] = 0;

                tmp_float = (float) (d5(e)) / 100.0;

                if (o_id > 2100) {
                    ol_st.set_int(0, o_id);
                    ol_st.set_int(1, o_d_id);
                    ol_st.set_int(2, o_w_id);
                    ol_st.set_int(3, ol);
                    ol_st.set_int(4, ol_i_id);
                    ol_st.set_int(5, ol_supply_w_id);
                    ol_st.set_date(6, date);
                    ol_st.set_int(7, ol_quantity);
                    ol_st.set_float(8, tmp_float);
                    ol_st.set_value(9, ol_dist_info);
                }else{
                    ol_st.set_int(0, o_id);
                    ol_st.set_int(1, o_d_id);
                    ol_st.set_int(2, o_w_id);
                    ol_st.set_int(3, ol);
                    ol_st.set_int(4, ol_i_id);
                    ol_st.set_int(5, ol_supply_w_id);
                    ol_st.set_date(6, date);
                    ol_st.set_int(7, ol_quantity);
                    ol_st.set_float(8, ol_amount);
                    ol_st.set_value(9, ol_dist_info);
                }
                ol_st.execute_update();
            }
        }
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void load_customer(int w_id, int d_id,
        pg_connection &con, int CUST_PER_DIST){
    std::clog << "-->--> Loading Table: customer, Max Item: " << CUST_PER_DIST << std::endl;
    std::clog << "-->--> Loading Table: history, Max Item: " << CUST_PER_DIST << std::endl;
    try{
        int c_id;
        int c_d_id;
        int c_w_id;

        char c_first[17];
        char c_middle[3];
        char c_last[17];
        char c_street_1[21];
        char c_street_2[21];
        char c_city[21];
        char c_state[3];
        char c_zip[10];
        char c_phone[17];
        PG::Date c_since;
        char c_credit[3];

        float c_discount;
        float c_balance;
        char c_data[501];

        char h_data[25];

        long million_sec = c_since.get_million_seconds();

        extern std::default_random_engine e;
        std::uniform_int_distribution<int> d(0, 1);
        std::uniform_int_distribution<int> d1(0, 50);
        std::uniform_int_distribution<long> d2(0L, 630720000L);

        parameter_type cus_types[] = {int_type, int_type, int_type, text_type,
                                  text_type, text_type, text_type, text_type,
                                  text_type, text_type, text_type, text_type, date_type,
                                  text_type, int_type, numeric_type, numeric_type,
                                  numeric_type, int_type, int_type, text_type};
        std::string cus_sql = "insert into customer(c_id,c_d_id,c_w_id,c_first,c_middle,c_last,"
                             "c_street_1,c_street_2,c_city,c_state,c_zip,c_phone,c_since,c_credit,"
                             "c_credit_lim,c_discount,c_balance,c_ytd_payment,c_payment_cnt,c_delivery_cnt,"
                             "c_data) values($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14,$15,$16,$17,$18,$19,$20,$21)";
        pg_prepared_statement cus_st = con.prepared_statement(cus_sql, cus_types);

        parameter_type his_types[] = {int_type, int_type, int_type, int_type,
                                      int_type, date_type, numeric_type, text_type};
        std::string his_sql = "insert into history(h_c_id,h_c_d_id,h_c_w_id,h_d_id,h_w_id,h_date,h_amount,h_data) "
                              "values($1,$2,$3,$4,$5,$6,$7,$8)";
        pg_prepared_statement his_st = con.prepared_statement(his_sql, his_types);

        for (c_id = 1; c_id <= CUST_PER_DIST; c_id++) {
            c_d_id = d_id;
            c_w_id = w_id;

            c_first[RANDOM_GEN::make_alpha_string(8, 16, c_first)] = 0;
            c_middle[0] = 'O';
            c_middle[1] = 'E';
            c_middle[2] = 0;

            if (c_id <= 1000) {
                RANDOM_GEN::Lastname(c_id - 1, c_last);
            } else {
                RANDOM_GEN::Lastname(RANDOM_GEN::NURand(255, 0, 999), c_last);
            }

            RANDOM_GEN::make_address(c_street_1, c_street_2, c_city, c_state, c_zip);
            c_phone[RANDOM_GEN::make_number_string(16, 16, c_phone)] = 0;

            if (d(e))
                c_credit[0] = 'G';
            else
                c_credit[0] = 'B';
            c_credit[1] = 'C';
            c_credit[2] = 0;

            c_discount = ((float) d1(e)) / 100.0;
            c_balance = -10.0;
            c_data[RANDOM_GEN::make_alpha_string(300, 500, c_data) ] = 0;

            cus_st.set_int(0, c_id);
            cus_st.set_int(1, c_d_id);
            cus_st.set_int(2, c_w_id);
            cus_st.set_value(3, c_first);
            cus_st.set_value(4, c_middle);
            cus_st.set_value(5, c_last);
            cus_st.set_value(6, c_street_1);
            cus_st.set_value(7, c_street_2);
            cus_st.set_value(8, c_city);
            cus_st.set_value(9, c_state);
            cus_st.set_value(10, c_zip);
            cus_st.set_value(11, c_phone);
            PG::Date tmp_date(million_sec - d2(e));
            cus_st.set_date(12, tmp_date);
            cus_st.set_value(13, c_credit);
            cus_st.set_int(14, 50000);
            cus_st.set_float(15, c_discount);
            cus_st.set_float(16, c_balance);
            cus_st.set_float(17, 30000.0);
            cus_st.set_int(18, 6500);
            cus_st.set_int(19, 7500);
            cus_st.set_value(20, c_data);
            cus_st.execute_update();

            h_data[RANDOM_GEN::make_alpha_string(12, 24, h_data)] = 0;

            his_st.set_int(0, c_id);
            his_st.set_int(1, c_d_id);
            his_st.set_int(2, c_w_id);
            his_st.set_int(3, d_id);
            his_st.set_int(4, w_id);
            his_st.set_date(5, tmp_date);
            his_st.set_float(6, 10.0);
            his_st.set_value(7, h_data);
            his_st.execute_update();
        }
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void load_stock(int w_id,
        pg_connection &con, int max_num){
    std::clog << "--> Loading Table: stock, Max Item: " << max_num << std::endl;
    try{
        int s_i_id;
        int s_w_id;
        int s_quantity;

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
        char s_data[51];

        int sdatasiz;
        int orig[max_num + 1];
        int pos;

        s_w_id = w_id;

        for (int i = 0; i < max_num / 10; i++){
            orig[i] = 0;
        }
        extern std::default_random_engine e;
        std::uniform_int_distribution<int> d(0, max_num);
        for (int i = 0; i < max_num / 10; i++) {
            do {
                pos = d(e);
            } while (orig[pos]);
            orig[pos] = 1;
        }
        std::uniform_int_distribution<int> d1(10, 100);
        parameter_type types[] = {int_type, int_type, int_type, text_type,
                                  text_type, text_type, text_type, text_type,
                                  text_type, text_type, text_type, text_type,
                                  text_type,text_type};
        std::string sql = "insert into stock(s_i_id,s_w_id,s_quantity,s_dist_01,s_dist_02,"
                          "s_dist_03,s_dist_04,s_dist_05,s_dist_06,s_dist_07,s_dist_08,s_dist_09,s_dist_10,s_data) "
                          "values($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14)";
        pg_prepared_statement st = con.prepared_statement(sql, types);

        pg_statement tmp_st = con.create_statement();
        pg_resultset res =  tmp_st.execute_query("select i_id from item");
        int count = 0;
        while(res.has_next() && (count ++) < max_num){
            const char *i_id = res.get_value(0);
            s_i_id = atoi(i_id);
            s_quantity = d1(e);
            s_dist_01[RANDOM_GEN::make_alpha_string(24, 24, s_dist_01)] = 0;
            s_dist_02[RANDOM_GEN::make_alpha_string(24, 24, s_dist_02)] = 0;
            s_dist_03[RANDOM_GEN::make_alpha_string(24, 24, s_dist_03)] = 0;
            s_dist_04[RANDOM_GEN::make_alpha_string(24, 24, s_dist_04)] = 0;
            s_dist_05[RANDOM_GEN::make_alpha_string(24, 24, s_dist_05)] = 0;
            s_dist_06[RANDOM_GEN::make_alpha_string(24, 24, s_dist_06)] = 0;
            s_dist_07[RANDOM_GEN::make_alpha_string(24, 24, s_dist_07)] = 0;
            s_dist_08[RANDOM_GEN::make_alpha_string(24, 24, s_dist_08)] = 0;
            s_dist_09[RANDOM_GEN::make_alpha_string(24, 24, s_dist_09)] = 0;
            s_dist_10[RANDOM_GEN::make_alpha_string(24, 24, s_dist_10)] = 0;
            sdatasiz = RANDOM_GEN::make_alpha_string(26, 50, s_data);
            s_data[sdatasiz] = 0;

            if (orig[s_i_id]) {
                std::uniform_int_distribution<int> d2(0, sdatasiz - 8);
                pos = d2(e);
                s_data[pos] = 'o';
                s_data[pos + 1] = 'r';
                s_data[pos + 2] = 'i';
                s_data[pos + 3] = 'g';
                s_data[pos + 4] = 'i';
                s_data[pos + 5] = 'n';
                s_data[pos + 6] = 'a';
                s_data[pos + 7] = 'l';
            }
            st.set_int(0, s_i_id);
            st.set_int(1, s_w_id);
            st.set_int(2, s_quantity);
            st.set_value(3, s_dist_01);
            st.set_value(4, s_dist_02);
            st.set_value(5, s_dist_03);
            st.set_value(6, s_dist_04);
            st.set_value(7, s_dist_05);
            st.set_value(8, s_dist_06);
            st.set_value(9, s_dist_07);
            st.set_value(10, s_dist_08);
            st.set_value(11, s_dist_09);
            st.set_value(12, s_dist_10);
            st.set_value(13, s_data);
            st.execute_update();
        }
    }catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}


void generate_item(){
    using namespace std;
    clog << "Generating Table: item, Max Item: " << STOCK_PER_WAREHOUSE << endl;
    try{
        int i_id;
        int i_im_id;
        char i_name[25];
        float i_price;
        char i_data[51];
        int idatasiz;
        int orig[STOCK_PER_WAREHOUSE + 1];
        int pos;
        std::uniform_int_distribution<int> d(0, STOCK_PER_WAREHOUSE);
        extern std::default_random_engine e;
        for (int i = 0; i < STOCK_PER_WAREHOUSE / 10; i++){
            orig[i] = 0;
        }
        for (int i = 0; i < STOCK_PER_WAREHOUSE / 10; i++) {
            do {
                pos = d(e);
            } while (orig[pos]);
            orig[pos] = 1;
        }
        std::uniform_int_distribution<int> d1(1, 10000);
        std::uniform_int_distribution<int> d2(100, 10000);

        char buffer[1000];
        getcwd(buffer, 1000);
        string path = string(buffer) + "/item.tbl";
        ofstream out(path.c_str(), ios_base::out | ios_base::app);
        if (!out.is_open()){
            cerr << "open failure" << endl;
            return;
        }

        for (i_id = 1; i_id <= STOCK_PER_WAREHOUSE; i_id++) {
            i_im_id = d1(e);
            i_name[RANDOM_GEN::make_alpha_string(14, 24, i_name)] = 0;
            i_price = (d2(e)) / 100.0;
            idatasiz = RANDOM_GEN::make_alpha_string(26, 50, i_data);
            i_data[idatasiz] = 0;
            if (orig[i_id]) {
                std::uniform_int_distribution<int> d2(0, idatasiz - 8);
                pos = d2(e);
                i_data[pos] = 'o';
                i_data[pos + 1] = 'r';
                i_data[pos + 2] = 'i';
                i_data[pos + 3] = 'g';
                i_data[pos + 4] = 'i';
                i_data[pos + 5] = 'n';
                i_data[pos + 6] = 'a';
                i_data[pos + 7] = 'l';
            }
            out << i_id << "|" << i_im_id << "|" << i_name << "|" << i_price << "|" << i_data << endl;
        }
        out.close();
    }catch(exception &e){
        cout << e.what() << endl;
    }
}

void generate_stock(int w_id, int max_num);

void generate_district(int w_id, int DIST_PER_WARE);

void generate_warehouse(int max_num){
    using namespace std;
    clog << "Generating Table: warehouse, Max Item: " << max_num << endl;
    try{
        char buffer[1000];
        getcwd(buffer, 1000);
        string path = string(buffer) + "/warehouse.tbl";
        ofstream out(path.c_str(), ios_base::out | ios_base::app);
        if (!out.is_open()){
            cerr << "open failure" << endl;
            return;
        }

        extern default_random_engine e;
        uniform_real_distribution<double> d(10L, 20L);
        char w_name[11];
        char w_street_1[21];
        char w_street_2[21];
        char w_city[21];
        char w_state[3];
        char w_zip[10];
        for (int i = 1; i <= max_num; i ++){
            RANDOM_GEN::make_address(w_street_1, w_street_2, w_city, w_state, w_zip);
            w_name[RANDOM_GEN::make_alpha_string(6, 10, w_name)] = 0;

            out << i << "|" << w_name << "|" << w_street_1
                << "|" << w_street_2 << "|" << w_city << "|"
                << w_state << "|" << w_zip << "|" << d(e) / 100.0
                << "|" << 30000.0 << endl;
            generate_stock(i, STOCK_PER_WAREHOUSE);
            generate_district(i, DISTRICT_PER_WAREHOUSE);
        }
        out.close();
    }catch (exception &e){
        cout << e.what() << endl;
    }
}

void generate_customer(int w_id, int d_id, int CUST_PER_DIST);
void generate_order(int w_id, int d_id, int ORD_PER_DIST);

void generate_district(int w_id, int DIST_PER_WARE){
    std::clog << "--> Generating Table: district, Max Item: " << DIST_PER_WARE << std::endl;
    try{
        int d_id;
        int d_w_id;

        char d_name[11];
        char d_street_1[21];
        char d_street_2[21];
        char d_city[21];
        char d_state[3];
        char d_zip[10];

        float d_tax;
        float d_ytd;
        int d_next_o_id;

        d_w_id = w_id;
        d_ytd = 30000.0;
        d_next_o_id = 3001L;
        extern std::default_random_engine e;
        std::uniform_real_distribution<float> d(10, 20);

        char buffer[1000];
        getcwd(buffer, 1000);
        std::string path = std::string(buffer) + "/district.tbl";
        std::ofstream out(path.c_str(), std::ios_base::out | std::ios_base::app);
        if (!out.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }

        for (d_id = 1; d_id <= DIST_PER_WARE; d_id++) {

            d_name[RANDOM_GEN::make_alpha_string(6L, 10L, d_name)] = 0;
            RANDOM_GEN::make_address(d_street_1, d_street_2, d_city, d_state, d_zip);

            d_tax = d(e) / 100.0;


            out << d_id << "|" << d_w_id << "|" << d_name << "|"
                << d_street_1 << "|" << d_street_2 << "|" << d_city
                << "|" << d_state << "|" << d_zip << "|" << d_tax
                << "|" << d_ytd << "|" << d_next_o_id << std::endl;
            generate_customer(w_id, d_id, CUSTOMER_PER_DISTRICT);
            generate_order(w_id, d_id, ORDER_PER_DISTRICT);
        }
        out.close();
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void generate_order(int w_id, int d_id, int ORD_PER_DIST){
    std::clog << "-->--> Generating Table: order, Max Item: " << ORD_PER_DIST << std::endl;
    std::clog << "-->--> Generating Table: new_order, Max Item: " << ORD_PER_DIST << std::endl;
    std::clog << "-->--> Generating Table: order_line, Max Item: " << ORD_PER_DIST << std::endl;
    try{
        int o_id;
        int o_c_id;
        int o_d_id;
        int o_w_id;
        int o_carrier_id;
        int o_ol_cnt;
        int ol;
        int ol_i_id;
        int ol_supply_w_id;
        int ol_quantity;
        float ol_amount;
        char ol_dist_info[25];
        float  tmp_float;

        PG::Date o_entry_d;
        long seconds = o_entry_d.get_million_seconds();

        o_d_id = d_id;
        o_w_id = w_id;

        extern std::default_random_engine e;
        std::uniform_int_distribution<int> d(1, CUSTOMER_PER_DISTRICT);
        std::uniform_int_distribution<int> d1(1, 10);
        std::uniform_int_distribution<int> d2(5, 15);
        std::uniform_int_distribution<long> d3(0L, 630720000L);
        std::uniform_int_distribution<long> d4(1, STOCK_PER_WAREHOUSE);
        std::uniform_int_distribution<long> d5(10, 10000);

        char buffer[1000];
        getcwd(buffer, 1000);

        std::string path = std::string(buffer) + "/order.tbl";
        std::ofstream order(path.c_str(), std::ios_base::out | std::ios_base::app);
        if (!order.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }

        std::string path1 = std::string(buffer) + "/new_order.tbl";
        std::ofstream new_order(path1.c_str(), std::ios_base::out | std::ios_base::app);
        if (!new_order.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }

        std::string path2 = std::string(buffer) + "/order_line.tbl";
        std::ofstream order_line(path2.c_str(), std::ios_base::out | std::ios_base::app);
        if (!order_line.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }
        for (o_id = 1; o_id <= ORD_PER_DIST; o_id++) {
            o_c_id = d(e);
            o_carrier_id = d1(e);
            o_ol_cnt = d2(e);
            PG::Date date(seconds - d3(e));
            if (o_id > 2100) {

                order << o_id << "|" << o_d_id << "|" << o_w_id << "|"
                      << o_c_id << "|" << date.get_local_date_string()
                      << "|" << o_carrier_id << "|" << o_ol_cnt
                      << "|" << 5000 << std::endl;

                new_order << o_id << "|" << o_d_id << "|" << o_w_id << std::endl;
            } else {
                order << o_id << "|" << o_d_id << "|" << o_w_id << "|"
                      << o_c_id << "|" << date.get_local_date_string()
                      << "|" << o_carrier_id << "|" << o_ol_cnt
                      << "|" << 5000 << std::endl;
            }
            for (ol = 1; ol <= o_ol_cnt; ol++) {
                /* Generate Order Line Data */
                ol_i_id = d4(e);
                ol_supply_w_id = o_w_id;
                ol_quantity = 5;
                ol_amount = 0.0;

                ol_dist_info[RANDOM_GEN::make_alpha_string(24, 24, ol_dist_info)] = 0;

                tmp_float = (float) (d5(e)) / 100.0;

                if (o_id > 2100) {
                    order_line << o_id << "|" << o_d_id  << "|" << o_w_id << "|"
                               << ol << "|" << ol_i_id << "|" << ol_supply_w_id << "|" << date.get_local_date_string()
                               << "|" << ol_quantity << "|" << tmp_float << "|" << ol_dist_info << std::endl;

                }else{
                    order_line << o_id << "|" << o_d_id  << "|" << o_w_id << "|"
                               << ol << "|" << ol_i_id << "|" << ol_supply_w_id << "|" << date.get_local_date_string()
                               << "|" << ol_quantity << "|" << ol_amount << "|" << ol_dist_info << std::endl;
                }
            }
        }
        order.close();
        new_order.close();
        order_line.close();
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void generate_customer(int w_id, int d_id, int CUST_PER_DIST){
    std::clog << "-->--> Generating Table: customer, Max Item: " << CUST_PER_DIST << std::endl;
    std::clog << "-->--> Generating Table: history, Max Item: " << CUST_PER_DIST << std::endl;
    try{
        int c_id;
        int c_d_id;
        int c_w_id;

        char c_first[17];
        char c_middle[3];
        char c_last[17];
        char c_street_1[21];
        char c_street_2[21];
        char c_city[21];
        char c_state[3];
        char c_zip[10];
        char c_phone[17];
        PG::Date c_since;
        char c_credit[3];

        float c_discount;
        float c_balance;
        char c_data[501];

        char h_data[25];

        long million_sec = c_since.get_million_seconds();

        extern std::default_random_engine e;
        std::uniform_int_distribution<int> d(0, 1);
        std::uniform_int_distribution<int> d1(0, 50);
        std::uniform_int_distribution<long> d2(0L, 630720000L);

        char buffer[1000];
        getcwd(buffer, 1000);

        std::string path = std::string(buffer) + "/history.tbl";
        std::ofstream history(path.c_str(), std::ios_base::out | std::ios_base::app);
        if (!history.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }

        std::string path1 = std::string(buffer) + "/customer.tbl";
        std::ofstream customer(path1.c_str(), std::ios_base::out | std::ios_base::app);
        if (!customer.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }

        for (c_id = 1; c_id <= CUST_PER_DIST; c_id++) {
            c_d_id = d_id;
            c_w_id = w_id;

            c_first[RANDOM_GEN::make_alpha_string(8, 16, c_first)] = 0;
            c_middle[0] = 'O';
            c_middle[1] = 'E';
            c_middle[2] = 0;

            if (c_id <= 1000) {
                RANDOM_GEN::Lastname(c_id - 1, c_last);
            } else {
                RANDOM_GEN::Lastname(RANDOM_GEN::NURand(255, 0, 999), c_last);
            }

            RANDOM_GEN::make_address(c_street_1, c_street_2, c_city, c_state, c_zip);
            c_phone[RANDOM_GEN::make_number_string(16, 16, c_phone)] = 0;

            if (d(e))
                c_credit[0] = 'G';
            else
                c_credit[0] = 'B';
            c_credit[1] = 'C';
            c_credit[2] = 0;

            c_discount = ((float) d1(e)) / 100.0;
            c_balance = -10.0;
            c_data[RANDOM_GEN::make_alpha_string(300, 500, c_data) ] = 0;

            customer << c_id << "|" << c_d_id << "|" << c_w_id  << "|"
                     << c_first  << "|" << c_middle  << "|" << c_last
                     << "|" << c_street_1  << "|" << c_street_2
                     << "|" << c_city << "|" << c_state << "|" << c_zip << "|" << c_phone << "|";
            PG::Date tmp_date(million_sec - d2(e));
            customer << tmp_date.get_local_date_string() << "|" << c_credit
                     << "|" << 50000 << "|" << c_discount << "|"
                     << c_balance << "|" << 30000.0 << "|"
                     << 6500 << "|" << 7500 << "|" << c_data << std::endl;

            h_data[RANDOM_GEN::make_alpha_string(12, 24, h_data)] = 0;

            history << c_id << "|" << c_d_id << "|" << c_w_id
                    << "|" << d_id << "|" << w_id << "|"
                    << tmp_date.get_local_date_string() << "|" << 10.0 << "|"
                    << h_data << std::endl;
        }
        customer.close();
        history.close();
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void generate_stock(int w_id, int max_num){
    std::clog << "--> Generating Table: stock, Max Item: " << max_num << std::endl;
    try{
        int s_i_id;
        int s_w_id;
        int s_quantity;

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
        char s_data[51];

        int sdatasiz;
        int orig[max_num + 1];
        int pos;

        s_w_id = w_id;

        for (int i = 0; i < max_num / 10; i++){
            orig[i] = 0;
        }
        extern std::default_random_engine e;
        std::uniform_int_distribution<int> d(0, max_num);
        for (int i = 0; i < max_num / 10; i++) {
            do {
                pos = d(e);
            } while (orig[pos]);
            orig[pos] = 1;
        }
        std::uniform_int_distribution<int> d1(10, 100);

        char buffer[1000];
        getcwd(buffer, 1000);
        std::string path1 = std::string(buffer) + "/stock.tbl";
        std::ofstream stock(path1.c_str(), std::ios_base::out | std::ios_base::app);
        if (!stock.is_open()){
            std::cerr << "open failure" << std::endl;
            return;
        }

        int count = 0;
        while((count ++) < max_num){
            s_i_id = count;
            s_quantity = d1(e);
            s_dist_01[RANDOM_GEN::make_alpha_string(24, 24, s_dist_01)] = 0;
            s_dist_02[RANDOM_GEN::make_alpha_string(24, 24, s_dist_02)] = 0;
            s_dist_03[RANDOM_GEN::make_alpha_string(24, 24, s_dist_03)] = 0;
            s_dist_04[RANDOM_GEN::make_alpha_string(24, 24, s_dist_04)] = 0;
            s_dist_05[RANDOM_GEN::make_alpha_string(24, 24, s_dist_05)] = 0;
            s_dist_06[RANDOM_GEN::make_alpha_string(24, 24, s_dist_06)] = 0;
            s_dist_07[RANDOM_GEN::make_alpha_string(24, 24, s_dist_07)] = 0;
            s_dist_08[RANDOM_GEN::make_alpha_string(24, 24, s_dist_08)] = 0;
            s_dist_09[RANDOM_GEN::make_alpha_string(24, 24, s_dist_09)] = 0;
            s_dist_10[RANDOM_GEN::make_alpha_string(24, 24, s_dist_10)] = 0;
            sdatasiz = RANDOM_GEN::make_alpha_string(26, 50, s_data);
            s_data[sdatasiz] = 0;

            if (orig[s_i_id]) {
                std::uniform_int_distribution<int> d2(0, sdatasiz - 8);
                pos = d2(e);
                s_data[pos] = 'o';
                s_data[pos + 1] = 'r';
                s_data[pos + 2] = 'i';
                s_data[pos + 3] = 'g';
                s_data[pos + 4] = 'i';
                s_data[pos + 5] = 'n';
                s_data[pos + 6] = 'a';
                s_data[pos + 7] = 'l';
            }
            stock << s_i_id << "|" << s_w_id << "|" << s_quantity << "|"
                  << s_dist_01 << "|" << s_dist_02 << "|" << s_dist_03 << "|"
                  << s_dist_04 << "|" << s_dist_05 << "|" << s_dist_06
                  << "|" << s_dist_07 << "|" << s_dist_08 << "|"
                  << s_dist_09 << "|" << s_dist_10 << "|" << s_data << std::endl;
        }
        stock.close();
    }catch (std::exception &e){
        std::cout << e.what() << std::endl;
    }
}
