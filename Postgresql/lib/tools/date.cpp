//
// Created by Administrator on 2019/3/27.
//

#include "date.h"
#include <ctime>
#include <iostream>
#include <iomanip>
#include "global_tools.h"

namespace PG_DATE {
    void Date::reset_time() {
        time_t time_stamp = time(NULL);
        this->million_seconds = time_stamp;
        tm *current_date = localtime(&time_stamp);
        this->year = current_date->tm_year + 1900;
        this->month = current_date->tm_mon + 1;
        this->day = current_date->tm_mday;
        this->hours = current_date->tm_hour;
        this->minutes = current_date->tm_min;
        this->seconds = current_date->tm_sec;
        this->format = format_24_hours;
    }

    bool Date::is_leap_year(int dest_year) {
        if (dest_year < 0) {
            return false;
        }
        if (dest_year % 400 == 0 || (dest_year % 4 == 0 && dest_year % 100 != 0)) {
            return true;
        } else {
            return false;
        }
    }

    Date::Date() {
        std::cout << "Hello,world" << std::endl;
        this->reset_time();
    }

    Date::Date(long million_seconds) {
        if (million_seconds < 0) {
            this->reset_time();
        } else {
            this->million_seconds = million_seconds;
            tm *current_date = localtime(&million_seconds);
            this->year = current_date->tm_year + 1900;
            this->month = current_date->tm_mon + 1;
            this->day = current_date->tm_mday;
            this->hours = current_date->tm_hour;
            this->minutes = current_date->tm_min;
            this->seconds = current_date->tm_sec;
            this->format = format_24_hours;
        }
    }

    Date::Date(int year, int month, int day, int hours,
               int minutes, int seconds, date_format format) {
        if (year < 0 || month < 0 || day < 0 || hours < 0 || minutes < 0 || seconds < 0) {
            this->reset_time();
        } else {
            this->year = year;
            this->month = month;
            this->hours = hours;
            this->day = day;
            this->minutes = minutes;
            this->seconds = seconds;
            this->format = format;
            int interval_day = 0;
            for (int i = 1970; i < this->year; i++) {
                interval_day += this->is_leap_year(i) ? 366 : 365;
            }
            for (int i = 1; i < this->month; i++) {
                switch (i) {
                    case 1:
                        interval_day += 31;
                        break;
                    case 2:
                        interval_day += 27;
                        break;
                    case 3:
                        interval_day += 31;
                        break;
                    case 5:
                        interval_day += 31;
                        break;
                    case 7:
                        interval_day += 31;
                        break;
                    case 8:
                        interval_day += 31;
                        break;
                    case 10:
                        interval_day += 31;
                        break;
                    case 12:
                        interval_day += 31;
                        break;
                    default:
                        interval_day += 30;
                        break;
                }
            }
            interval_day += this->is_leap_year(year) ? 1 : 0;
            interval_day += (this->day - 1);
            long interval_sec = hours * 3600 + minutes * 60 + seconds;
            this->million_seconds = interval_day * 24 * 3600 + interval_sec;
        }
    }

    Date::~Date() {}

    void Date::set_date_format(date_format format) {
        this->format = format;
    }

    long Date::get_million_seconds() {
        return this->million_seconds;
    }

    Date Date::operator-(const Date &date) {
        long interval = (this->million_seconds > date.million_seconds) ?
                        (this->million_seconds - date.million_seconds) : (date.million_seconds - this->million_seconds);
        Date res = Date(interval);
        return res;
    }

    Date Date::operator+(const Date &date) {
        Date res = Date(this->million_seconds + date.million_seconds);
        return res;
    }

    std::ostream &operator<<(std::ostream &os, const Date &date) {
        if (date.format == format_24_hours) {
            os << std::setfill('0') << date.year << "-"
               << std::setw(2) << date.month << "-"
               << std::setw(2) << date.day << " "
               << std::setw(2) << date.hours << ":"
               << std::setw(2) << date.minutes << ":"
               << std::setw(2) << date.seconds;
        } else {
            os << std::setfill('0') << date.year << "-"
               << std::setw(2) << date.month << "-"
               << std::setw(2) << date.day << " "
               << std::setw(2) << ((date.hours > 12) ? (date.hours - 12) : date.hours) << ":"
               << std::setw(2) << date.minutes << ":"
               << std::setw(2) << date.seconds << ((date.hours >= 12) ? "pm" : "am");
        }
        return os;
    }

    const std::string Date::get_local_date_string() const{
        std::string year_str = parseInt(this->year);
        std::string month_str = parseInt(this->month);
        std::string day_str = parseInt(this->day);
        std::string hours_str = parseInt(this->hours);
        std::string minutes_str = parseInt(this->minutes);
        std::string seconds_str = parseInt(this->seconds);
        return year_str + "-" + month_str + "-" + day_str + " " + hours_str + ":" + minutes_str + ":" + seconds_str;
    }

    Date::operator long() const {
        return long(this->million_seconds);
    }
}
