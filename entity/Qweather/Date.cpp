//
// Created by zeng on 23-12-19.
//

#include <cstdio>
#include <ctime>
#include "Date.h"
#include "fmt/format.h"

QWeather::Date::Date(std::string_view date) {
    // 从2021-11-16格式的std::string_view中解析出年月日
    sscanf(date.data(), "%d-%d-%d", &this->year, &this->month, &this->day);
}

QWeather::Date::Date(int year, int month, int day) : year(year), month(month), day(day) {
}

bool QWeather::Date::IsToday() {
    // 获取当前时间
    auto now{time(nullptr)};
    auto *local{localtime(&now)};
    // 比较年月日是否相等
    auto nowadays_year{local->tm_year + 1900};
    auto nowadays_month{local->tm_mon + 1};
    auto nowadays_day{local->tm_mday};
    return this->year == nowadays_year && this->month == nowadays_month && this->day == nowadays_day;
}

std::ostream &QWeather::operator<<(std::ostream &os, const QWeather::Date &date) {
    os << fmt::format("{:04d}-{:02d}-{:02d}", date.year, date.month, date.day);
    return os;
}
