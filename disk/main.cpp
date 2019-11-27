#include <vector>
#include <math.h>
#include <iostream>
#include <unordered_map>
#include "util/bufferutil.h"
#include "domain/pg_buffer.h"
#include "util/databaseutil.h"
#include "util/configutil.h"

int main() {
    using namespace std;
    pg_database database = databaseutil::getDatabase("tpcc");
    cout << database << endl;
    vector<pg_buffer> val = bufferutil::getBuffers(database);
    cout << "val.size(): " << val.size() << endl;
    cout << val[12] << endl;
    cout << pow(1 - val[12].usageratio, 118) << endl;
    unordered_map<string, string> map = configutil::getConfig();
    string tmp = map["TPS"];
    cout << (int)((double) 100000 / (double) 31) << endl;
    return 0;
}