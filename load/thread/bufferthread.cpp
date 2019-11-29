//
// Created by ilmare on 11/28/19.
//

#include "bufferthread.h"
#include "../global_define.h"
#include <pg_lib/pg_connection.h>
#include <pg_lib/pg_statement.h>
#include <pg_lib/pg_resultset.h>
#include <tools/global_tools.h>
#include <unistd.h>

void *buffer_record(void *param){
    using namespace std;
    thread_arg *arg = (thread_arg *)param;
    unordered_map<string, string> config = arg->config;
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string databasestr = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), databasestr.c_str(), port.c_str());

    string sql = "select datid from pg_stat_database where datname='" + databasestr + "'";
    pg_statement st = con.create_statement();
    pg_resultset resultset1 = st.execute_query(sql);
    resultset1.has_next();
    string databaseid = resultset1.get_string(0);
    resultset1.close();

    timeval start;
    gettimeofday(&start, NULL);
    string fileName = parseInt(start.tv_sec * 1000000 + start.tv_usec);
    fstream out;
    out.open(config["DISK_FILE"] + "record_" + fileName + ".csv", std::ios_base::app | std::ios_base::out);
    if (!out.is_open()){
        exit(EXIT_FAILURE);
    }

    sql = "select isdirty, count(*) from pg_buffercache where reldatabase=" + databaseid + " group by isdirty";
    st = con.create_statement();
    timeval current;
    gettimeofday(&current, NULL);
    int count = 0;
    double sleep_margin = atoi(config["BUFFER_RECORD_INTERVAL"].c_str());
    long maxTime = EXECUTE_TIME + BUFFER_WATCH_EXTEND;

    while(current.tv_sec - start.tv_sec < maxTime && ++count){
        pg_resultset resultset = st.execute_query(sql);
        unordered_map<string, string> tmp;
        while(resultset.has_next()){
            pair<string, string> p(resultset.get_string(0), resultset.get_string(1));
            tmp.insert(p);
        }
        resultset.close();
        auto fpage = tmp.find("f");
        auto tpage = tmp.find("t");
        gettimeofday(&current, NULL);
        long fpagerec = 0, tpagerec = 0;
        if (fpage != tmp.end()){
            fpagerec = atoi(fpage->second.c_str());
        }
        if (tpage != tmp.end()){
            tpagerec = atoi(tpage->second.c_str());
        }
        out << (current.tv_sec - start.tv_sec) * 1000000 + (current.tv_usec - start.tv_usec) << ","
            << fpagerec << "," << tpagerec << "," << fpagerec + tpagerec
            << endl;
        sleep(sleep_margin);
    }
    con.close();
    out.close();
    PG::Date date;
    pthread_t t = pthread_self();
    clog << date << " [INFO] " << "buffer_record@["
         << (void *)&t << "], total running time is " << current.tv_sec - start.tv_sec
         << "s, run " << count << " times" << endl;
}