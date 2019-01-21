#include <iostream>
#include <libpq-fe.h>

int main() {
    using namespace std;
    const char *conn_info = "postgresql://ilmare@10.69.35.174:5432/testdb?connect_timeout=10&password=123456";
    const char *conInfo = "host=10.69.35.174 port=5432 dbname=testdb connect_timeout=10 password=123456 user=ilmare";
    PGconn *conn = PQconnectdb(conn_info);
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
    const char *sql = "explain select * from blog_visit limit 30";
    PGresult *result_set = PQexec(conn, sql);
    int column_count = PQnfields(result_set);
    int row_count = PQntuples(result_set);
    for (unsigned i = 0; i < row_count; i ++){
        for (unsigned j = 0; j < column_count; j ++){
            char *field_name = PQfname(result_set, j);
            cout << field_name << ": " << PQgetvalue(result_set, i, j) << " ";
        }
        cout << endl;
    }
    PQclear(result_set);
    PQfinish(conn);
    return 0;
}