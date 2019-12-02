#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <tools/date.h>
#include <unordered_map>
#include <tools/global_tools.h>
#include "load/driver.h"
#include "global_define.h"
#include "tpcc/sequence.h"
#include "util/configutil.h"
#include "thread/TPSthread.h"
#include "thread/mainthread.h"
#include "thread/bufferthread.h"

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

int num_ware = 100;

int main(int argn, char *argv[]) {
    using namespace std;

    timeval tv;
    gettimeofday(&tv, NULL);
    unordered_map<string, string> config = configutil::getConfig();
    string file_name = config["DATA_FILE"] + "originlog_" + parseInt(tv.tv_sec * 1000000 + tv.tv_usec) + ".csv";

    bool disk_statistic_flag = !(config["WATCH_TPS_FLAG"] == "false");
    bool buffer_flag = !(config["BUFFER_RECORD_FLAG"] == "false");

    int thread_num = 1;

    thread_arg arg(config, thread_num, file_name, tv);
    //===============================================tps监控线程==============================================
    pthread_t watch_thread;
    if (disk_statistic_flag){
        pthread_create(&watch_thread, NULL, tps_record, (void *) &arg);
    }
    //===============================================监控线程完===============================================
    //=============================================buffer监控线程=============================================
    pthread_t buffer_thread;
    if (buffer_flag){
        pthread_create(&buffer_thread, NULL, buffer_record, (void *) &arg);
    }
    //=============================================buffer监控线程完============================================
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
    if (disk_statistic_flag){
        pthread_join(watch_thread, NULL);
    }
    extern int total_count, count_time;
    PG::Date date;
    std::clog << date << " [INFO] Total execution time is " << count_time
              << "s, total count is " << total_count << ", tps is "
              << ((double)total_count / count_time)
              << std::endl;

    if (buffer_flag){
        pthread_join(buffer_thread, NULL);
    }
    return 0;
}