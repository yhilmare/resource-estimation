#include <vector>
#include <math.h>
#include <iostream>
#include <unordered_map>
#include "util/bufferutil.h"
#include "domain/pg_buffer.h"
#include "util/databaseutil.h"
#include "util/configutil.h"
#include "util/pageflushutil.h"
#include <cmath>
#include <pg_lib/pg_connection.h>
#include <pg_lib/pg_statement.h>
#include <pg_lib/pg_resultset.h>

unordered_map<string, string> func(const pg_database &database){
    unordered_map<string, string> config = configutil::getConfig();
    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string databasestr = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), databasestr.c_str(), port.c_str());

    string sql = "select isdirty, count(*) from pg_buffercache where reldatabase=" + database.datid + " group by isdirty";
    pg_statement st = con.create_statement();
    pg_resultset resultset = st.execute_query(sql);
    unordered_map<string, string> map;
    while(resultset.has_next()){
        pair<string, string> p(resultset.get_string(0), resultset.get_string(1));
        map.insert(p);
    }
    resultset.close();
    con.close();
    return map;
}

int main() {
    using namespace std;
    pg_database database = databaseutil::getDatabase("tpc");
    cout << database << endl;
    bufferutil bu = bufferutil(database);
    vector<pg_buffer> val = bu.getBuffers();
    cout << val.size() << endl;




    unordered_map<string, string> map = func(database);
    for(unordered_map<string, string>::iterator iter = map.begin(); iter != map.end(); iter ++){
        cout << (*iter).first << ":" << iter->second << endl;
    }
//    unordered_map<string, string> config = configutil::getConfig();
//    pageflushutil pf = pageflushutil(val, config);
//    cout << pf << endl;
//    pf.calculate();
    return 0;
}