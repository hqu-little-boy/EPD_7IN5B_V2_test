//
// Created by zeng on 23-12-19.
//

#include <cstdio>
#include <ctime>
#include "Time_MinAndSec.h"

QWeather::Time_MinAndSec::Time_MinAndSec(int hour, int minute): hour(hour), minute(minute) {
}

QWeather::Time_MinAndSec::Time_MinAndSec(std::string_view time) {
    // 从12:12格式的std::string_view中解析出时分
    sscanf(time.data(), "%d:%d", &this->hour, &this->minute);
}

bool QWeather::Time_MinAndSec::IsNow() {
    // 获取当前时间
    auto now{time(nullptr)};
    auto *local{localtime(&now)};
    // 比较时分是否相等
    auto nowadays_hour{local->tm_hour};
    auto nowadays_minute{local->tm_min};
    return this->hour == nowadays_hour && this->minute == nowadays_minute;
}
