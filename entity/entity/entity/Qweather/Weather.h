//
// Created by zeng on 23-12-19.
//

#ifndef CPR_TEST_WEATHER_H
#define CPR_TEST_WEATHER_H

#include "../Pic/Config.h"
#include "Date.h"
#include "Time_MinAndSec.h"

namespace QWeather {
    //天气类,用于存储天气信息,使用正确定义的Config类从指定的API获取天气信息
    class Weather {
    public:
        Weather(const EinkPic::Config &config);

        //使用config类中的属性，按和urlTodayWeather的api地址，获取今天天气信息
        bool GetTodayWeather();

        //使用config类中的属性，按和url24hWeather的api地址，获取下一个小时天气信息
        bool GetNextHourWeather();

        //使用config类中的属性，按和urlNowWeather的api地址，获取实时天气信息
        bool GetNowWeather();

        //判断一小时内是否会下雨
        bool IsRainingInOneHour();

        //获取最高温度，根据fxDate判断是否是今天，如果是今天，返回tempMax，否则调用一次GetTodayWeather返回tempMin
        int GetTempMax();

        //获取最低温度，根据fxDate判断是否是今天，如果是今天，返回tempMin，否则调用一次GetTodayWeather返回tempMin
        int GetTempMin();

        //获取白天天气图标，根据fxDate判断是否是今天，如果是今天，返回iconDay，否则调用一次GetTodayWeather返回iconDay
        int GetIconDay();

        //获取白天天气文字，根据fxDate判断是否是今天，如果是今天，返回textDay，否则调用一次GetTodayWeather返回textDay
        std::string_view GetTextDay();

        //获取第二天的最高温度，根据fxDate判断是否是今天，如果是今天，返回tempMax，否则调用一次GetTodayWeather返回tempMin
        int GetTempMaxNext();
        //获取第二天的最低温度，根据fxDate判断是否是今天，如果是今天，返回tempMin，否则调用一次GetTodayWeather返回tempMin
        int GetTempMinNext();
        //获取第二天的白天天气图标，根据fxDate判断是否是今天，如果是今天，返回iconDay，否则调用一次GetTodayWeather返回iconDay
        int GetIconDayNext();
        //获取第二天的白天天气文字，根据fxDate判断是否是今天，如果是今天，返回textDay，否则调用一次GetTodayWeather返回textDay
        std::string_view GetTextDayNext();
    private:
        QWeather::Date fxDate;//日期
        int tempMax;//最高温度
        int tempMin;//最低温度
        int iconDay;//白天天气图标
        std::string textDay;//白天天气文字

        int tempMaxNext;//最高温度
        int tempMinNext;//最低温度
        int iconDayNext;//白天天气图标
        std::string textDayNext;//白天天气文字

        std::string nowWeatherText;//实时天气文字
        std::string nextWeatherText;//下一个小时天气文字
        const EinkPic::Config &config;//配置类

        //使用config类中的属性，按照fmt::format格式化指定参数的格式，获取今天天气API地址
        static constexpr std::string_view urlTodayWeather = "https://devapi.qweather.com/v7/weather/3d?location={}&key={}";
        //使用config类中的属性，按照fmt::format格式化指定参数的格式，获取24h天气API地址
        static constexpr std::string_view url24hWeather = "https://devapi.qweather.com/v7/weather/24h?location={}&key={}";
        //使用config类中的属性，按照fmt::format格式化指定参数的格式，获取实时天气API地址
        static constexpr std::string_view urlNowWeather = "https://devapi.qweather.com/v7/weather/now?location={}&key={}";
    };
}

#endif //CPR_TEST_WEATHER_H
