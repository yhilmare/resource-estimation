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

int main() {
    using namespace std;
    pg_database database = databaseutil::getDatabase("tpc");
    cout << database << endl;
    bufferutil bu = bufferutil(database);
    vector<pg_buffer> val = bu.getBuffers();

    unordered_map<string, string> config = configutil::getConfig();
    pageflushutil pf = pageflushutil(val, config);
    cout << pf << endl;
    pf.calculate();
    return 0;
}