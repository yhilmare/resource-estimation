#include <iostream>
#include <unordered_map>
#include <sys/unistd.h>
#include <tools/global_tools.h>
#include <pg_lib/pg_connection.h>
#include <string>
#include "util/databassutil.h"
#include "domain/pg_buffer.h"
#include <vector>
#include "util/bufferutil.h"

int main() {

    using namespace std;
    pg_database database = databassutil::getDatabase("tpcc");
    cout << database << endl;
    vector<pg_buffer> val = bufferutil::getBuffers(database);
    cout << val.size() << endl;
    double total = .0;
    for(vector<pg_buffer>::iterator iter = val.begin(); iter != val.end(); iter ++){
        total += (*iter).usagecount;
    }
    for(int i = 0; i < val.size(); i ++){
        pg_buffer item = val[i];
        val[i].usageratio = item.usagecount / total;
    }
    cout << val[156] << endl;
    return 0;
}