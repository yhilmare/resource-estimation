//
// Created by ilmare on 11/21/19.
//

#ifndef DISK_CONFIGUTIL_H
#define DISK_CONFIGUTIL_H

#include <unordered_map>
#include <string>

using namespace std;

class configUtil {
public:
    static unordered_map<string, string> getConfig();
};


#endif //DISK_CONFIGUTIL_H
