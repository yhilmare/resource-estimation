//
// Created by ilmare on 11/21/19.
//

#include "bufferutil.h"
#include "../util/configUtil.h"
#include <pg_lib/pg_connection.h>

vector<pg_buffer> bufferutil::getBuffers(const pg_database &database) {
    unordered_map<string, string> config = configUtil::getConfig();
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
    while(resultset.has_next()){
        pg_buffer buffer = pg_buffer(&resultset);
        val.push_back(buffer);
    }
    resultset.close();
    con.close();
    return val;
}
