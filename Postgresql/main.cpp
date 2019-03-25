#include <iostream>
#include <libpq-fe.h>
#include <regex>
#include <vector>
#include <iterator>
#include "lib/pg_lib/pg_connection.h"
#include "lib/pg_lib/exception/conn_info_nullpointer_exception.h"

void postgresql_test(){
    using namespace std;
    const char *conn_info = "postgresql://ilmare@10.69.35.174:5432/tpcD?connect_timeout=10&password=123456";
    const char *conInfo = "host=10.69.35.174 port=5432 dbname=TPCD connect_timeout=10 password=123456 user=ilmare";
    PGconn *conn = PQconnectdb(conInfo);
    try{
        cout << PQparameterStatus(conn, "integer_datetimes") << endl;
    }catch(const exception &e){
        cout << e.what() << endl;
    }
    if (PQstatus(conn) != CONNECTION_OK){
        cout << "Connection failed!" << endl;
        PQfinish(conn);
        exit(-1);
    }
    const char * pre = "prepare test(date) as select l_orderkey,l_partkey,l_shipdate,l_comment from lineitem where l_shipdate = $1 limit 5 offset 100;";
    const char *sql = "execute test(date '1994-10-24');";
    const char *sql1 = "deallocate prepare all;";
//    const char *sql = "select l_orderkey,l_partkey,l_shipdate,l_comment from lineitem where l_shipdate = date '1994-10-23' limit 50 offset 100";
    PQexec(conn, pre);
    try{
        PGresult *result_set = PQexec(conn, sql);
        int column_count = PQnfields(result_set);
        int row_count = PQntuples(result_set);
        for (unsigned i = 0; i < column_count; i ++){
            printf("%-15s", PQfname(result_set, i));
        }
        cout << endl;
        for (unsigned i = 0; i < row_count; i ++){
            for (unsigned j = 0; j < column_count; j ++){
                printf("%-15s", PQgetvalue(result_set, i, j));
            }
            cout << endl;
        }
        PQexec(conn, sql1);
        PQclear(result_set);
        PQfinish(conn);
    }catch (const exception &s){
        std::cout << "err" << std::endl;

        std::cout << s.what() << std::endl;
    }

}
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
int main(int arg_n, char *arg_v[]) {
    using namespace std;
    postgresql_test();
//    try{
//        pg_connection con("ilmare", "123456", "10.69.35.174", "TPCD", "5432", 10);
//    }catch(const exception &e){
//        cout << e.what() << endl;
//    }

//    const char *Url = "postgresql://ilmare@10.69.35.174/TPCD?connect_timeout=10&password=123456";
//    PGconn *connection = PQconnectdb(Url);
//    if (PQstatus(connection) != CONNECTION_OK){
//        cerr << "connection failure" << endl;
//        PQfinish(connection);
//        exit(-1);
//    }
//    const char *sql = "explain analyse select * from lineitem where L_SHIPDATE = date '1996-11-24'";
//    PGresult *result = PQexec(connection, sql);
//    int column_count = PQnfields(result);
//    int tuple_count = PQntuples(result);
//    for (int i = 0; i < tuple_count; i ++){
//        for (int j = 0; j < column_count; j ++){
//            char *field_name = PQfname(result, j);
//            cout << field_name << ": " << PQgetvalue(result, i, j) << endl;
//        }
//    }
//    PQfinish(connection);
    return 0;
}