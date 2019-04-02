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

void load_warehouse(std::unordered_map<std::string, std::string> &config,
        int max_num){
    using namespace std;
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];
    pg_connection con(user.c_str(), password.c_str(), host.c_str(), database.c_str(), port.c_str());
    try{
        con.set_auto_commit(false);
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
            char w_tax_s[100];
            sprintf(w_tax_s, "%.5f", d(e) / 100.0);
            char w_ytd_s[100];
            sprintf(w_ytd_s, "%.2f", 30000.0);
            string w_id_s = parseInt(i);
            st.set_value(0, w_id_s.c_str());
            string w_name_s = get_pg_string(w_name);
            st.set_value(1, w_name_s.c_str());
            string w_street_1_s = get_pg_string(w_street_1);
            st.set_value(2, w_street_1_s.c_str());
            string w_street_2_s = get_pg_string(w_street_2);
            st.set_value(3, w_street_2_s.c_str());
            string w_city_s = get_pg_string(w_city);
            st.set_value(4, w_city_s.c_str());
            string w_state_s = get_pg_string(w_state);
            st.set_value(5, w_state_s.c_str());
            string w_zip_s = get_pg_string(w_zip);
            st.set_value(6, w_zip_s.c_str());
            st.set_value(7, w_tax_s);
            st.set_value(8, w_ytd_s);
            st.execute_update();
            cout << w_id_s.c_str() << "|" << w_name_s.c_str() << "|" << w_street_1_s.c_str()
                 << "|" << w_street_2_s.c_str() << "|" << w_city_s.c_str() << "|" << w_state_s.c_str() << "|"
                 << w_zip_s.c_str() << "|" << w_tax_s << "|" << w_ytd_s << endl;
        }
        con.commit();
    }catch (exception &e){
        cout << e.what() << endl;
        con.roll_back();
    }
}