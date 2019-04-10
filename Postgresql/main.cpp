#include <iostream>
#include <regex>
#include <vector>
#include <exception>
#include <cstring>
#include <fstream>
#include <direct.h>
#include <pthread.h>
#include <random>
#include <time.h>
#include "global_define.h"
#include "lib/pg_lib/pg_connection.h"
#include "lib/pg_lib/exception/conn_info_nullpointer_exception.h"
#include "lib/pg_lib/pg_statement.h"
#include "lib/pg_lib/pg_resultset.h"
#include "lib/pg_lib/pg_prepared_statement.h"
#include "lib/tools/date.h"
#include "lib/tools/global_tools.h"
#include "load/data_load.h"


void regex_test(){
    using namespace std;
    regex pattern = regex(R"pattern(\w+(@)\w+\.com)pattern");
    string msg = "yh_swjtu@163.com是我一个人1193830957@qq.com";
    smatch match;
    cout << regex_match(msg, match, pattern) << endl;
    smatch search;
    cout << regex_search(msg, search, pattern) << endl;
    cout << search.str() << endl;
    cout << "=============================" << endl;
    sregex_iterator iter(msg.begin(), msg.end(), pattern);
    sregex_iterator end;
    for(; iter != end; iter ++){
        cout << iter->str() << endl;
    }
    cout << "=====================" << endl;
    sregex_token_iterator token_iter = sregex_token_iterator(msg.begin(), msg.end(), pattern);
    sregex_token_iterator token_end;
    for(; token_iter != token_end; token_iter ++){
        cout << token_iter->str() << " - " << *token_iter << endl;
    }
}

void load_data(){
    generate_item();
    generate_warehouse(10);
//    char buffer[1000];
//    getcwd(buffer, 1000);
//    unordered_map<string, string>
//            map = parse_properties_file(string(buffer) + "/config/pg_config.properties");
//    load_item(map);
//    load_warehouse(map, 10);
}

int main(int argn, char *argv[]) {
    using namespace std;
    pg_connection con("ilmare", "123456",
                      "10.69.35.174", "tpcc", "5432");
    clock_t start = clock();
    con.set_auto_commit(false);
    string sql1 = "select count(*) from order_line where ol_o_id=1";
    pg_statement st = con.create_statement();
    pg_resultset res = st.execute_query(sql1);
    cout << (clock() - start) << endl;
    while(res.has_next()){
        for(int i = 0; i < res.get_column_count(); i ++){
            cout << res.get_value(i) << " ";
        }
        cout << endl;
    }
    string sql2 = "select count(*) from order_line where ol_o_id=2";
    pg_resultset res1 = st.execute_query(sql2);
    cout << (clock() - start) << endl;
    while(res1.has_next()){
        for(int i = 0; i < res1.get_column_count(); i ++){
            cout << res1.get_value(i) << " ";
        }
        cout << endl;
    }
    con.commit();
    return 0;
}