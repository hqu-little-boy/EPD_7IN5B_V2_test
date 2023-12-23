//
// Created by zeng on 23-12-19.
//

#ifndef CPR_TEST_DATE_H
#define CPR_TEST_DATE_H

#include <string_view>

namespace QWeather {
    // 对应一个日期类，可以解析2021-11-16格式的日期，也可以使用三个数字的年月日格式
    class Date {
    public:
        Date() = default;
        Date(std::string_view date);
        Date(int year, int month, int day);
        // 比较日期是否是今天
        bool IsToday();
        //重载<<运算符，使得可以使用std::cout输出日期
        friend std::ostream &operator<<(std::ostream &os, const Date &date);
    private:
        int year;
        int month;
        int day;
    };
}
#endif //CPR_TEST_DATE_H
