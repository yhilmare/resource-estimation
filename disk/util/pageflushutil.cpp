//
// Created by ilmare on 11/22/19.
//

#include "pageflushutil.h"

pageflushutil::pageflushutil() {
    throw exception();
}

pageflushutil::pageflushutil(vector<pg_buffer> &val, unordered_map<string, string> &config) {
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
    this->t = new double[this->buffer_length];
    this->sum = new double[this->buffer_length];
    this->cache = new double[this->buffer_length];
}

pageflushutil& pageflushutil::operator=(const pageflushutil &instance) {
    if (this == &instance){
        return *this;
    }
    if (this->p_1 != NULL){
        delete[] this->p_1;
    }
    if (this->p_2 != NULL){
        delete[] this->p_2;
    }
    if (this->p_3 != NULL){
        delete[] this->p_3;
    }
    if (this->t != NULL){
        delete[] this->t;
    }
    if (this->sum != NULL){
        delete[] this->sum;
    }
    if (this->cache != NULL){
        delete[] this->cache;
    }
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
    this->sum = new double[this->buffer_length];
    this->cache = new double[this->buffer_length];
    return *this;
}

pageflushutil::pageflushutil(const pageflushutil &instance) {
    if (this->p_1 != NULL){
        delete[] this->p_1;
    }
    if (this->p_2 != NULL){
        delete[] this->p_2;
    }
    if (this->p_3 != NULL){
        delete[] this->p_3;
    }
    if (this->t != NULL){
        delete[] this->t;
    }
    if (this->sum != NULL){
        delete[] this->sum;
    }
    if (this->cache != NULL){
        delete[] this->cache;
    }
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
    this->sum = new double[this->buffer_length];
    this->cache = new double[this->buffer_length];
}

pageflushutil::~pageflushutil() {
    if (this->p_1 != NULL){
        delete[] this->p_1;
    }
    if (this->p_2 != NULL){
        delete[] this->p_2;
    }
    if (this->p_3 != NULL){
        delete[] this->p_3;
    }
    if (this->t != NULL){
        delete[] this->t;
    }
    if (this->sum != NULL){
        delete[] this->sum;
    }
    if (this->cache != NULL){
        delete[] this->cache;
    }
}
