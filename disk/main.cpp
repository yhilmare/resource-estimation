#include <iostream>
#include <unordered_map>
#include <sys/unistd.h>
#include <tools/global_tools.h>
#include <pg_lib/pg_connection.h>
#include <string>

int main() {

    using namespace std;

    char buffer[1000];
    getcwd(buffer, 1000);
    unordered_map<string, string> config =
            parse_properties_file(string(buffer) + "/config/pg_config.properties");

    string host = config["PG_HOST"];
    string password = config["PG_PASSWORD"];
    string timout = config["PG_TIMEOUT"];
    string database = config["PG_DATABASE"];
    string user = config["PG_USER"];
    string port = config["PG_PORT"];

    pg_connection con(user.c_str(), password.c_str(),
                      host.c_str(), database.c_str(), port.c_str());
    string sql = "select ctid,* from warehouse";
    pg_statement st = con.create_statement();
    pg_resultset resultset = st.execute_query(sql);
    int count = resultset.get_column_count();
    while(resultset.has_next()){
        for(int i = 0; i < count; i ++){
            cout << resultset.get_string(i) << " ";
        }
        cout << endl;
    }
    resultset.close();
    con.close();
    return 0;
}