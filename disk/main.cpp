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
#include <math.h>

int main() {

    using namespace std;
    pg_database database = databassutil::getDatabase("tpcc");
    cout << database << endl;
    vector<pg_buffer> val = bufferutil::getBuffers(database);
    cout << "val.size(): " << val.size() << endl;
    double total = .0;
    for(vector<pg_buffer>::iterator iter = val.begin(); iter != val.end(); iter ++){
        total += (*iter).usagecount;
    }
    cout << "total: " << total << endl;
    for(int i = 0; i < val.size(); i ++){
        pg_buffer item = val[i];
        val[i].usageratio = item.usagecount / total;
    }
    cout << val[15001] << endl;
    cout << pow(1 - val[15001].usageratio, 118) << endl;
    return 0;
}