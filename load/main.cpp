#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include "load/driver.h"
#include "global_define.h"
#include <tools/date.h>
#include "./tpcc/sequence.h"
#include <tools/global_tools.h>
#include <unordered_map>

//void regex_test(){
//    using namespace std;
//    regex pattern = regex(R"pattern(\w+(@)\w+\.com)pattern");
//    string msg = "是我一个人@qq.com";
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

int main(int argn, char *argv[]) {
    using namespace std;

    timeval tv;
    gettimeofday(&tv, NULL);
    string final = parseInt(tv.tv_sec * 1000000 + tv.tv_usec);
    char buffer[1000];
    getcwd(buffer, 1000);
    unordered_map<string, string> config =
            parse_properties_file(string(buffer) + "/config/pg_config.properties");
    string file_name = config["DATA_FILE"] + "originlog_" + final + ".csv";
    int thread_num = 5;
    thread_arg arg(config, thread_num, file_name, tv);
    pthread_t ts[thread_num];
    for (int i = 0; i < thread_num; i ++){
        pthread_t t1;
        pthread_create(&t1, NULL, thread_main, (void *) &arg);
        ts[i] = t1;
    }
//    thread_main(&arg);
    for(int i = 0; i < thread_num; i ++){
        pthread_join(ts[i], NULL);
    }
    extern int total_count, count_time;
    PG::Date date;
    std::clog << date << " [INFO] Total execution time is " << count_time
              << "s, total count is " << total_count << ", tps is "
              << ((double)total_count / count_time)
              << std::endl;
    return 0;
}

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

    //seq_init(int n, int p, int o, int d, int s)
    seq_init(10, 25, 15, 10, 15);

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