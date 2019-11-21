//
// Created by ilmare on 11/21/19.
//

#include "objectfactory.h"


objectfactory::objectfactory() {}

objectfactory &objectfactory::getInstance() {
    static objectfactory instance = objectfactory();
    return instance;
}

void* objectfactory::getInstanceByName(string name) {
    unordered_map<string, instanceFunc>::iterator iter = map.find(name);
    if (iter == map.end()){
        return NULL;
    }else{
        return iter->second();
    }
}

void objectfactory::registryFunc(string name, instanceFunc func) {
    pair<string, instanceFunc> p(name, func);
    map.insert(p);
}
