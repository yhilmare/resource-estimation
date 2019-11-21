//
// Created by ilmare on 11/21/19.
//

#include "configUtil.h"
#include <sys/unistd.h>
#include <tools/global_tools.h>

unordered_map<string, string> configUtil::getConfig() {
    char buffer[1000];
    getcwd(buffer, 1000);
    unordered_map<string, string> config =
            parse_properties_file(string(buffer) + "/config/pg_config.properties");
    return config;
}
