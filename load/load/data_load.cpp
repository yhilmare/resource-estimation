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
#include <tools/global_tools.h>
#include <pg_lib/pg_connection.h>
#include <pg_lib/pg_prepared_statement.h>

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
