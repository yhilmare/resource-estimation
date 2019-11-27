//
// Created by ilmare on 11/27/19.
//

#include "databaseutil.h"
#include <iostream>
#include "../util/configutil.h"
#include <pg_lib/pg_connection.h>
#include <string>

pg_database databaseutil::getDatabase(string name) {

    unordered_map<string, string> config = configutil::getConfig();
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), database.c_str(), port.c_str());

    string sql = "select * from pg_stat_database";
    pg_statement st = con.create_statement();
    pg_resultset resultset = st.execute_query(sql);
    pg_database returnObj;
    while(resultset.has_next()){
        if (resultset.get_string(1) != name){
            continue;
        }
        returnObj.datid = resultset.get_string(0);
        returnObj.datname = resultset.get_string(1);
        returnObj.blks_read = resultset.get_int(5);
        returnObj.blks_hit = resultset.get_int(6);
        returnObj.deadlocks = resultset.get_int(15);
        returnObj.blk_read_time = resultset.get_float(16);
        returnObj.blk_write_time = resultset.get_float(17);
    }
    resultset.close();
    con.close();
    return returnObj;
}