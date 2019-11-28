//
// Created by ilmare on 11/28/19.
//

#include "TPSthread.h"
#include "../global_define.h"
#include <pg_lib/pg_connection.h>
#include <pg_lib/pg_statement.h>
#include <pg_lib/pg_resultset.h>
#include <tools/global_tools.h>
#include <unistd.h>

void *tps_record(void *param){

    thread_arg *arg = (thread_arg *)param;
    timeval start;
    gettimeofday(&start, NULL);
    long interval = EXECUTE_TIME;
    extern disk_statistic disk_obj;

    std::unordered_map<std::string, std::string> config = arg->config;
    double sleep_margin = atoi(config["WATCH_INTERVAL"].c_str());

    std::fstream out;
    std::string final = parseInt(start.tv_sec * 1000000 + start.tv_usec);
    std::string disk_name = config["DISK_FILE"] + "tps_" + final + ".csv";
    out.open(disk_name, std::ios_base::app | std::ios_base::out);
    if (!out.is_open()){
        exit(EXIT_FAILURE);
    }
    timeval current;
    gettimeofday(&current, NULL);
    while(current.tv_sec - start.tv_sec < interval){
        pthread_mutex_lock(&disk_obj.disk_mutex);
        PG::Date date;
        long watch_interval = disk_obj.current_timer - disk_obj.pre_timer;
        if (watch_interval != 0){
            double live_tps = disk_obj.interval_count / (watch_interval / 1000000.0);
            std::clog << date << " [INFO] watch interval is " << watch_interval
                      << "us, watch transaction is " << disk_obj.interval_count
                      << ", live tps is " << live_tps << std::endl;
            long tmp_time = disk_obj.current_timer - (start.tv_sec * 1000000 + start.tv_usec);
            out << tmp_time << "," << live_tps << std::endl;
            disk_obj.interval_count = 0;
            disk_obj.pre_timer = disk_obj.current_timer;
        }
        pthread_mutex_unlock(&disk_obj.disk_mutex);
        gettimeofday(&current, NULL);
        sleep(sleep_margin);
    }
    out.close();
}