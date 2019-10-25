#include <regex>
#include <vector>
#include <random>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <exception>
#include <pthread.h>
#include "load/driver.h"
#include "global_define.h"
#include "lib/tools/date.h"
#include "load/data_load.h"
#include "./tpcc/sequence.h"
#include "lib/tools/global_tools.h"
#include "lib/pg_lib/pg_statement.h"
#include "lib/pg_lib/pg_resultset.h"
#include "lib/pg_lib/pg_connection.h"
#include "./tpcc/container/transaction_obj.h"
#include "lib/pg_lib/pg_prepared_statement.h"
#include "lib/pg_lib/exception/conn_info_nullpointer_exception.h"

//void regex_test(){
//    using namespace std;
//    regex pattern = regex(R"pattern(\w+(@)\w+\.com)pattern");
//    string msg = "yh_swjtu@163.com是我一个人1193830957@qq.com";
//    smatch match;
//    cout << regex_match(msg, match, pattern) << endl;
//    smatch search;
//    cout << regex_search(msg, search, pattern) << endl;
//    cout << search.str() << endl;
//    cout << "=============================" << endl;
//    sregex_iterator iter(msg.begin(), msg.end(), pattern);
//    sregex_iterator end;
//    for(; iter != end; iter ++){
//        cout << iter->str() << endl;
//    }
//    cout << "=====================" << endl;
//    sregex_token_iterator token_iter = sregex_token_iterator(msg.begin(), msg.end(), pattern);
//    sregex_token_iterator token_end;
//    for(; token_iter != token_end; token_iter ++){
//        cout << token_iter->str() << " - " << *token_iter << endl;
//    }
//}

int num_ware = 10;
void *thread_main(void *);

void load_data(){
    generate_item();
    generate_warehouse(num_ware);
//    char buffer[1000];
//    getcwd(buffer, 1000);
//    unordered_map<string, string>
//            map = parse_properties_file(string(buffer) + "/config/pg_config.properties");
//    load_item(map);
//    load_warehouse(map, num_ware);
}

int main(int argn, char *argv[]) {
    using namespace std;

    timeval tv;
    gettimeofday(&tv, NULL);
    string time = parseInt(tv.tv_usec);
    for(int i = 0; i < 6 - time.length(); i ++){
        time = "0" + time;
    }
    string final = parseInt(tv.tv_sec) + time;

    char buffer[1000];
    getcwd(buffer, 1000);
    unordered_map<string, string> config =
            parse_properties_file(string(buffer) + "/config/pg_config.properties");
    string file_name = config["DATA_FILE"] + "originlog_" + final + ".csv";
    int thread_num = 10;
    thread_arg arg(config, thread_num, file_name);

//    for (int i = 0; i < thread_num - 1; i ++){
//        pthread_t t1;
//        pthread_create(&t1, NULL, thread_main, (void *) &arg);
////        pthread_join(t1, NULL);
//    }
    thread_main((void *)&arg);
    return 0;
}
void *thread_main(void *param){
    using namespace std;

    pthread_t t = pthread_self();
    std::clog << "This is Thread: [" << t << "]@"
              << (void *)&t << ", function [thread_main]@"
              << (void *)thread_main << std::endl;

    thread_arg *arg = (thread_arg *)param;
    unordered_map<string, string> config = arg->config;
    int thread_num = arg->thread_num;

    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    seq_init(10, 10, 1, 1, 1);

    pg_connection con(user.c_str(), password.c_str(),
            host.c_str(), database.c_str(), port.c_str());
    vector<pg_prepared_statement> val;
    for (int i = 0; i < sizeof(SQL_STRING) / sizeof(string); i ++){
        string sql = SQL_STRING[i];
        const parameter_type *type = SQL_PARAMETER_TYPE[i];
        pg_prepared_statement tmp_st = con.prepared_statement(sql, type);
        val.push_back(tmp_st);
    }
    driver(con, val, thread_num, arg->obj);
    return param;
}