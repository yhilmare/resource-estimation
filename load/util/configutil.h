//
// Created by ilmare on 11/29/19.
//

#ifndef LOAD_CONFIGUTIL_H
#define LOAD_CONFIGUTIL_H

#include <unordered_map>

using namespace std;
class configutil {
public:
    static unordered_map<string, string> getConfig();
};


#endif //LOAD_CONFIGUTIL_H
