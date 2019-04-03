//
// Created by Administrator on 2019/3/29.
//

#ifndef POSTGRESQL_DATA_LOAD_H
#define POSTGRESQL_DATA_LOAD_H

#include <unordered_map>

namespace RANDOM_GEN {
    void make_address(char *w_street_1, char *w_street_2,
                      char *w_city, char *w_state, char *w_zip);

    int make_number_string(int x, int y, char str[]);

    int make_alpha_string(int x, int y, char str[]);

    void Lastname(int num, char *name);

    int NURand (unsigned A, unsigned x, unsigned y);
}

void load_warehouse(std::unordered_map<std::string, std::string> &config, int max_num=1000);

void load_item(std::unordered_map<std::string, std::string> &);

#endif //POSTGRESQL_DATA_LOAD_H
