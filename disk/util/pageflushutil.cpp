//
// Created by ilmare on 11/22/19.
//

#include "pageflushutil.h"
#include <cmath>

pageflushutil::pageflushutil(vector<pg_buffer> &val,
        unordered_map<string, string> &config):p_1(NULL), p_2(NULL),
        p_3(NULL), t(NULL), sum(NULL), cache(NULL), buffers(val) {
    unordered_map<string, string>::iterator tps_pair = config.find("TPS");
    unordered_map<string, string>::iterator log_length_pair = config.find("LOG_LENGTH");
    unordered_map<string, string>::iterator epsino_pair = config.find("EPSINO");
    unordered_map<string, string>::iterator max_flush_rate_pair = config.find("MAXFLUSHRATE");
    if (tps_pair == config.end() || log_length_pair == config.end()
    || epsino_pair == config.end() || max_flush_rate_pair == config.end()){
        throw  exception();
    }
    this->tps = atof(tps_pair->second.c_str());
    this->log_length = atof(log_length_pair->second.c_str());
    this->epsino = atof(epsino_pair->second.c_str());
    this->max_flush_rate = atof(max_flush_rate_pair->second.c_str());
    this->buffer_length = (int)val.size();
    this->period = (int)(this->log_length / this->tps);
    this->m = this->period - 1;
    if (this->m <= 0){
        throw exception();
    }
    this->p_1 = new double[this->buffer_length];
    this->p_2 = new double[this->buffer_length];
    this->p_3 = new double[this->buffer_length];
    for(int i = 0; i < this->buffer_length; i ++){
        *(this->p_1 + i) = .0;
        *(this->p_2 + i) = .0;
        *(this->p_3 + i) = 1.0;
    }
    this->tp = new double[this->buffer_length];
    this->t = new double[this->buffer_length];
    this->sum = new double[this->buffer_length];
    this->cache = new double[this->buffer_length];
}

pageflushutil& pageflushutil::operator=(const pageflushutil &instance) {
    if (this == &instance){
        return *this;
    }
    delete[] this->p_1, this->p_2, this->p_3;
    delete[] this->t, this->sum, this->cache, this->tp;
    this->tps = instance.tps;
    this->log_length = instance.log_length;
    this->epsino = instance.epsino;
    this->max_flush_rate = instance.max_flush_rate;
    this->buffer_length = instance.buffer_length;
    this->period = instance.period;
    this->m = instance.m;
    this->flush_rate = instance.flush_rate;
    this->avg_flush_rate = instance.avg_flush_rate;
    if (this->m <= 0){
        throw exception();
    }
    this->p_1 = new double[this->buffer_length];
    this->p_2 = new double[this->buffer_length];
    this->p_3 = new double[this->buffer_length];
    this->t = new double[this->buffer_length];
    this->tp = new double[this->buffer_length];
    this->sum = new double[this->buffer_length];
    this->cache = new double[this->buffer_length];
    for(int i = 0; i < instance.buffer_length; i ++){
        *(this->p_1 + i) = instance.p_1[i];
        *(this->p_2 + i) = instance.p_2[i];
        *(this->p_3 + i) = instance.p_3[i];
        *(this->t + i) = instance.t[i];
        *(this->sum + i) = instance.sum[i];
        *(this->cache + i) = instance.cache[i];
        *(this->tp + i) = instance.tp[i];
    }
    return *this;
}

pageflushutil::pageflushutil(const pageflushutil &instance): buffers(instance.buffers) {
    delete[] this->p_1, this->p_2, this->p_3;
    delete[] this->t, this->sum, this->cache, this->tp;
    this->tps = instance.tps;
    this->log_length = instance.log_length;
    this->epsino = instance.epsino;
    this->max_flush_rate = instance.max_flush_rate;
    this->buffer_length = instance.buffer_length;
    this->period = instance.period;
    this->m = instance.m;
    this->flush_rate = instance.flush_rate;
    this->avg_flush_rate = instance.avg_flush_rate;
    if (this->m <= 0){
        throw exception();
    }
    this->p_1 = new double[this->buffer_length];
    this->p_2 = new double[this->buffer_length];
    this->p_3 = new double[this->buffer_length];
    this->t = new double[this->buffer_length];
    this->tp = new double[this->buffer_length];
    this->sum = new double[this->buffer_length];
    this->cache = new double[this->buffer_length];
    for(int i = 0; i < instance.buffer_length; i ++){
        *(this->p_1 + i) = instance.p_1[i];
        *(this->p_2 + i) = instance.p_2[i];
        *(this->p_3 + i) = instance.p_3[i];
        *(this->t + i) = instance.t[i];
        *(this->sum + i) = instance.sum[i];
        *(this->cache + i) = instance.cache[i];
        *(this->tp + i) = instance.tp[i];
    }
}

pageflushutil::~pageflushutil() {
    delete[] this->p_1, this->p_2, this->p_3;
    delete[] this->t, this->sum, this->cache, this->tp;
}

ostream &operator<<(ostream &out, const pageflushutil &instance) {
    out << "[pageflushutil]@" << (void *)&instance;
    return out;
}

void pageflushutil::calculate() {
    for(int i = 0; i < this->buffer_length; i ++){
        pg_buffer item = buffers[i];
        t[i] = 1 - pow(1 - item.usageratio, tps);
        tp[i] = pow(1 - t[i], m);
        cache[i] = pow(1 - tps / log_length, m);
        if (t[i] == (tps / log_length)){
            cout << "impossible" << endl;
            sum[i] = m * tp[i] / (1 - t[i]);
        }else{
            sum[i] = (pow(1 - t[i], m) - pow(1 - tps / log_length, m)) / (tps / log_length - t[i]);
        }
    }
    for(int i = 0; i < 1200; i ++){
        cout << sum[i] << " ";
        if ((i + 1) % 10 == 0){
            cout << endl;
        }
    }
//    int iter = 0;
//    double d1 = 0;
//    while(abs(avg_flush_rate - flush_rate) > epsino){
//        avg_flush_rate = (iter * avg_flush_rate + flush_rate) / (double)(iter + 1);
//        iter ++;
//        for(int i = 0; i < buffer_length; i ++){
//            d1 += p_1[i];
//        }
//        flush_rate = min(d1 / (double)period, max_flush_rate);
//        for(int i = 0; i < buffer_length; i ++){
//            p_3[i] = p_3[i] * tp[i] + (tps / log_length) * p_1[i] * sum[i];
//            p_1[i] = p_1[i] * cache[i];
//            p_2[i] = 1 - p_1[i] - p_3[i];
//
//            p_3[i] = p_3[i] + p_1[i];
//            p_1[i] = p_2[i];
//            p_2[i] = 0;
//        }
//    }
}
