//
// Created by ilmare on 11/28/19.
//

#include "mainthread.h"
#include <pg_lib/pg_connection.h>
#include "../tpcc/sequence.h"
#include "../load/driver.h"
#include "../global_define.h"

void *thread_main(void *param){
    using namespace std;
    pthread_t t = pthread_self();
//    std::clog << "This is Thread: [" << t << "]@"
//              << (void *)&t << ", function [thread_main]@"
//              << (void *)thread_main << std::endl;

    thread_arg *arg = (thread_arg *)param;
    unordered_map<string, string> config = arg->config;
    int thread_num = arg->thread_num;

    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];
    bool flag = config["WATCH_TPS_FLAG"] == "false" ? false : true;
    //seq_init(int n, int p, int o, int d, int s)
    seq_init(900, 77, 8, 8, 8);
    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), database.c_str(), port.c_str());
//    vector<pg_prepared_statement> val;
//    for (int i = 0; i < sizeof(SQL_STRING) / sizeof(string); i ++){
//        string sql = SQL_STRING[i];
//        const parameter_type *type = SQL_PARAMETER_TYPE[i];
//        pg_prepared_statement tmp_st = con.prepared_statement(sql, type);
//        val.push_back(tmp_st);
//    }
    driver(con, thread_num, arg->obj, flag);
    return param;
}
