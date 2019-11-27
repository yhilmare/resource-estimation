//
// Created by ilmare on 11/27/19.
//

#include "configutil.h"
#include <sys/unistd.h>
#include <tools/global_tools.h>

unordered_map<string, string> configutil::getConfig() {
    char buffer[1000];
    getcwd(buffer, 1000);
    unordered_map<string, string> config =
            parse_properties_file(string(buffer) + "/config/pg_config.properties");
    return config;
}