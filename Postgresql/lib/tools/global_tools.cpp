//
// Created by Administrator on 2019/3/27.
//

#include "global_tools.h"
#include <vector>
#include <cstring>

const std::string parseInt(int num){
    using namespace std;
    vector<int> val;
    while(true){
        val.push_back(num % 10);
        num /= 10;
        if (!num){
            break;
        }
    }
    char *tmp = new char[val.size() + 1];
    for (int i = 0; i < val.size(); i ++){
        tmp[i] = val[val.size() - i - 1] + 48;
    }
    tmp[val.size()] = 0;
    std::string res = std::string(tmp);
    delete []tmp;
    return res;
}

const std::string get_pg_date_string(const PG_DATE::Date &d){
    return "date '" + d.get_local_date_string() + "'";
}

const std::string get_pg_string(const char *s){
    int length = strlen(s);
    char *tmp = new char[length + 3];
    tmp[0] = 39;
    for (int i = 0; i < length; i ++){
        tmp[1 + i] = s[i];
    }
    tmp[length + 1] = 39;
    tmp[length + 2] = 0;
    std::string res = std::string(tmp);
    delete []tmp;
    return res;
}
