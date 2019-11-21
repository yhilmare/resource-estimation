//
// Created by ilmare on 11/21/19.
//

#ifndef DISK_OBJECTFACTORY_H
#define DISK_OBJECTFACTORY_H

#include <unordered_map>
#include <string>

typedef void *(*instanceFunc)();

using namespace std;
class objectfactory {
private:
    unordered_map<string, instanceFunc> map;
    objectfactory();

public:
    static objectfactory &getInstance();

    void *getInstanceByName(string name);

    void registryFunc(string name, instanceFunc func);
};


#endif //DISK_OBJECTFACTORY_H
