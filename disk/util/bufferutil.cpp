//
// Created by ilmare on 11/21/19.
//

#include "bufferutil.h"
#include "../util/configutil.h"
#include <fstream>
#include <tools/global_tools.h>
#include <pg_lib/pg_connection.h>
#include <unistd.h>


bufferutil::bufferutil(pg_database &instance): database(instance) {}

void bufferutil::recordBuffers(long interval, long delay) {
    unordered_map<string, string> config = configutil::getConfig();
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string databasestr = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    timeval start;
    gettimeofday(&start, NULL);
    string fileName = parseInt(start.tv_sec * 1000000 + start.tv_usec);
    fstream out;
    out.open(config["DISK_REC_PATH"] + "record_" + fileName + ".csv", std::ios_base::app | std::ios_base::out);
    if (!out.is_open()){
        exit(EXIT_FAILURE);
    }
    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), databasestr.c_str(), port.c_str());

    string sql = "select isdirty, count(*) from pg_buffercache where reldatabase=" + database.datid + " group by isdirty";
    pg_statement st = con.create_statement();
    timeval current;
    gettimeofday(&current, NULL);
    int count = 0;
    while(current.tv_sec - start.tv_sec < interval &&  ++count){
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
        sleep(delay);
    }
    con.close();
    out.close();
    PG::Date date;
    clog << date << " [INFO] " << "bufferutil::recordBuffers@["
         << (void *)this << "], total running time is " << current.tv_sec - start.tv_sec
         << "s, run " << count << " times" << endl;
}


vector<pg_buffer> bufferutil::getBuffers() {
    unordered_map<string, string> config = configutil::getConfig();
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string databasestr = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), databasestr.c_str(), port.c_str());

    string sql = "select * from pg_buffercache where reldatabase=" + database.datid;
    pg_statement st = con.create_statement();
    pg_resultset resultset = st.execute_query(sql);
    vector<pg_buffer> val;
    double total = .0;
    while(resultset.has_next()){
        total += resultset.get_int(7);
        pg_buffer buffer = pg_buffer(&resultset);
        val.push_back(buffer);
    }
    resultset.close();
    con.close();
    for(int i = 0; i < val.size(); i ++){
        pg_buffer item = val[i];
        val[i].usageratio = item.usagecount / total;
    }
    return val;
}
