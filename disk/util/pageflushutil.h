//
// Created by ilmare on 11/22/19.
//

#ifndef DISK_PAGEFLUSHUTIL_H
#define DISK_PAGEFLUSHUTIL_H

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include "../domain/pg_buffer.h"

using namespace std;
class pageflushutil {
private:
    double flush_rate = 0, avg_flush_rate = 50, max_flush_rate;
    double *p_1, *p_2, *p_3;
    double *t, *sum, *cache, *tp;
    double log_length, tps, epsino;
    int buffer_length, period, m;
    vector<pg_buffer> &buffers;
public:
    pageflushutil(vector<pg_buffer> &val, unordered_map<string, string> &config);
    pageflushutil(const pageflushutil &);

    pageflushutil &operator=(const pageflushutil &);

    friend ostream &operator<<(ostream &out, const pageflushutil &instance);

    void calculate();
    ~pageflushutil();
};

#endif //DISK_PAGEFLUSHUTIL_H
