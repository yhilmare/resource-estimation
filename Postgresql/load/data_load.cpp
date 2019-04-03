//
// Created by Administrator on 2019/3/29.
//

#include "data_load.h"
#include <iostream>
#include <cstring>
#include <random>
#include <unordered_map>
#include "../lib/pg_lib/pg_connection.h"
#include "../lib/pg_lib/pg_prepared_statement.h"
#include "../lib/tools/global_tools.h"

namespace RANDOM_GEN {
    std::default_random_engine e;
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

void load_item(std::unordered_map<std::string, std::string> &config, int max_num){
    using namespace std;
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];
    try{
        pg_connection con(user.c_str(), password.c_str(),
                          host.c_str(), database.c_str(), port.c_str());
        parameter_type types[] = {int_type, int_type, text_type, numeric_type, text_type};
        string sql = "insert into item(i_id,i_im_id,i_name,i_price,i_data) values($1,$2,$3,$4,$5)";
        pg_prepared_statement st = con.prepared_statement(sql, types);
        int i_id;
        int i_im_id;
        char i_name[25];
        float i_price;
        char i_data[51];
        int idatasiz;
        int orig[max_num + 1];
        int pos;
        std::uniform_int_distribution<int> d(0, max_num);
        extern std::default_random_engine e;
        for (int i = 0; i < max_num / 10; i++){
            orig[i] = 0;
        }
        for (int i = 0; i < max_num / 10; i++) {
            do {
                pos = d(e);
            } while (orig[pos]);
            orig[pos] = 1;
        }
        std::uniform_int_distribution<int> d1(1, 10000);
        std::uniform_int_distribution<int> d2(100, 10000);

        for (i_id = 1; i_id <= max_num; i_id++) {
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
    }catch(exception &e){
        cout << e.what() << endl;
    }
}
void load_stock(int w_id,
                pg_connection &con, int max_num);
void load_district(int w_id, pg_connection &con, int DIST_PER_WARE);
void load_warehouse(std::unordered_map<std::string, std::string> &config,
        int max_num){
    using namespace std;
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
        for (int i = 0; i < max_num; i ++){
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
            load_stock(i, con, 1000);
            load_district(i, con, 1000);
        }
    }catch (exception &e){
        cout << e.what() << endl;
    }
}

void load_district(int w_id, pg_connection &con, int DIST_PER_WARE){
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
        }
    }catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }
}

void load_stock(int w_id,
        pg_connection &con, int max_num){
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
        std::string sql = "insert into stock(s_i_id,s_w_id,s_quantity,s_dist_01,"
                          "s_dist_02,s_dist_03,s_dist_04,s_dist_05,s_dist_06,s_dist_07,"
                          "s_dist_08,s_dist_09,s_dist_10,s_data) "
                          "values($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11,$12,$13,$14)";
        pg_prepared_statement st = con.prepared_statement(sql, types);
        for (s_i_id = 1; s_i_id <= max_num; s_i_id++) {
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
