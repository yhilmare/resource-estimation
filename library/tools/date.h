//
// Created by Administrator on 2019/3/27.
//

#ifndef POSTGRESQL_DATE_H
#define POSTGRESQL_DATE_H

#include <iostream>
#include <iomanip>

namespace PG{
    enum date_format{
        format_24_hours,
        format_12_hours
    };
    class Date{
    private:
        long million_seconds;
        int year;
        int month;
        int day;
        int hours;
        int minutes;
        int seconds;
        date_format format;
        bool is_leap_year(int dest_year);
    public:
        Date();
        Date(long million_seconds);
        Date(int year,
                int month,
                int day,
                int hours=0,
                int minutes=0,
                int seconds=0,
                date_format format = format_24_hours);
        ~Date();
        void reset_time();
        void set_date_format(date_format format);
        long get_million_seconds();
        Date operator+(const Date &date);
        Date operator-(const Date &date);
        friend std::ostream &operator<<(std::ostream &os, const Date &date);
        explicit operator long() const;//这是C++中的强制类型转换函数

        const std::string get_local_date_string() const;
    };
}


#endif //POSTGRESQL_DATE_H
