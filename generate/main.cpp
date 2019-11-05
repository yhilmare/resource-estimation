#include <unistd.h>
#include <iostream>
#include "load/data_load.h"
#include <tools/global_tools.h>

int num_ware = 10;

int main(int argn, char *argv[]) {
    using namespace std;
    char buffer[1000];
    cout << "Please Select load Type:" << endl
         << "1.Generate Data File" << endl
         << "2.Load By Transaction" << endl;
    int result = -1;
    cin >> result;
    switch (result){
        case 1:
            generate_item();
            generate_warehouse(num_ware);
            break;
        case 2:
            getcwd(buffer, 1000);
            unordered_map<string, string>
                    map = parse_properties_file(string(buffer) + "/config/pg_config.properties");
            load_item(map);
            load_warehouse(map, num_ware);
            break;
    }
    return 0;
}