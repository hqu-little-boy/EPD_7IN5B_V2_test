//
// Created by zeng on 23-12-19.
//

#ifndef CPR_TEST_TIME_MINANDSEC_H
#define CPR_TEST_TIME_MINANDSEC_H

#include <string_view>

namespace QWeather {
    // 对应一个时间类，可以解析12:12格式的时间，也可以使用两个数字的时分格式
    class Time_MinAndSec {
    public:
        Time_MinAndSec() = default;
        Time_MinAndSec(std::string_view time);
        Time_MinAndSec(int hour, int minute);
        // 比较时间是否是现在
        bool IsNow();
    private:
        int hour;
        int minute;
    };
}

#endif //CPR_TEST_TIME_MINANDSEC_H
