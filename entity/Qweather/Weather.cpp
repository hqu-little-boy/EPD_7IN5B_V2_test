//
// Created by zeng on 23-12-19.
//

#include <iostream>
#include "Weather.h"
#include "cpr/cprtypes.h"
#include "cpr/cpr.h"
#include "fmt/format.h"

QWeather::Weather::Weather(const EinkPic::Config &config) : config(config) {
}

bool QWeather::Weather::GetTodayWeather() {
    // 使用config类中的属性，按照fmt::format格式化指定参数的格式，获取API地址
    std::string strTodayWeatherUrl{
            fmt::format(QWeather::Weather::urlTodayWeather, std::string(this->config.GetLocationCode()),
                        std::string(this->config.GetQWeatherKey()))};
    //打印API地址
    std::cout << "24hWeatherUrl: " << strTodayWeatherUrl << std::endl;
    // 创建一个cpr::Url对象
    auto cprUrlTodayWeather{cpr::Url{strTodayWeatherUrl}};
    // 创建一个GET请求
    auto response = cpr::Get(cprUrlTodayWeather);
    // 检查请求是否成功
    if (response.status_code == 200) {
        // 输出响应内容
        std::cout << "Response: " << response.text << std::endl;
        // 将响应内容转换为json对象
        // 响应格式：{"code":"200","updateTime":"2023-12-19T19:35+08:00","fxLink":"https://www.qweather.com/weather/beijing-101010100.html","daily":[{"fxDate":"2023-12-19","sunrise":"07:30","sunset":"16:54","moonrise":"12:10","moonset":"23:58","moonPhase":"峨眉月","moonPhaseIcon":"801","tempMax":"-4","tempMin":"-12","iconDay":"100","textDay":"晴","iconNight":"150","textNight":"晴","wind360Day":"339","windDirDay":"西北风","windScaleDay":"3-4","windSpeedDay":"20","wind360Night":"0","windDirNight":"北风","windScaleNight":"1-3","windSpeedNight":"16","humidity":"53","precip":"0.0","pressure":"1034","vis":"25","cloud":"0","uvIndex":"2"},{"fxDate":"2023-12-20","sunrise":"07:31","sunset":"16:54","moonrise":"12:36","moonset":"","moonPhase":"上弦月","moonPhaseIcon":"802","tempMax":"-6","tempMin":"-15","iconDay":"100","textDay":"晴","iconNight":"150","textNight":"晴","wind360Day":"315","windDirDay":"西北风","windScaleDay":"1-3","windSpeedDay":"16","wind360Night":"0","windDirNight":"北风","windScaleNight":"1-3","windSpeedNight":"3","humidity":"47","precip":"0.0","pressure":"1034","vis":"25","cloud":"0","uvIndex":"2"},{"fxDate":"2023-12-21","sunrise":"07:31","sunset":"16:55","moonrise":"13:00","moonset":"01:11","moonPhase":"盈凸月","moonPhaseIcon":"803","tempMax":"-5","tempMin":"-14","iconDay":"100","textDay":"晴","iconNight":"150","textNight":"晴","wind360Day":"315","windDirDay":"西北风","windScaleDay":"1-3","windSpeedDay":"3","wind360Night":"0","windDirNight":"北风","windScaleNight":"1-3","windSpeedNight":"3","humidity":"60","precip":"0.0","pressure":"1034","vis":"25","cloud":"0","uvIndex":"2"}],"refer":{"sources":["QWeather"],"license":["CC BY-SA 4.0"]}}
        auto json24hWeather{nlohmann::json::parse(response.text).at("daily").at(0)};
        std::cout << "Response: " << json24hWeather.dump(3) << std::endl;
        // 对json对象进行解析，将解析后的数据赋给类中对应的属性
        this->fxDate = Date(json24hWeather.at(0).at("fxDate").get<std::string>());
        // 将json中的字符串转为数字赋给类中对应的属性
        this->tempMax = std::stoi(json24hWeather.at(0).at("tempMax").get<std::string>());
        this->tempMin = std::stoi(json24hWeather.at(0).at("tempMin").get<std::string>());
        this->iconDay = std::stoi(json24hWeather.at(0).at("iconDay").get<std::string>());
        this->textDay = json24hWeather.at(0).at("textDay").get<std::string>();
        // 打印解析后的数据
        std::cout << "fxDate: " << this->fxDate << std::endl;
        std::cout << "tempMax: " << this->tempMax << std::endl;
        std::cout << "tempMin: " << this->tempMin << std::endl;
        std::cout << "iconDay: " << this->iconDay << std::endl;
        std::cout << "textDay: " << this->textDay << std::endl;
        auto json24hWeatherNext{nlohmann::json::parse(response.text).at("daily").at(1)};
        // 将json中的字符串转为数字赋给类中对应的属性
        this->tempMaxNext = std::stoi(json24hWeatherNext.at(0).at("tempMax").get<std::string>());
        this->tempMinNext = std::stoi(json24hWeatherNext.at(0).at("tempMin").get<std::string>());
        this->iconDayNext = std::stoi(json24hWeatherNext.at(0).at("iconDay").get<std::string>());
        this->textDayNext = json24hWeatherNext.at(0).at("textDay").get<std::string>();
        // 打印解析后的数据
        std::cout << "tempMaxNext: " << this->tempMaxNext << std::endl;
        std::cout << "tempMinNext: " << this->tempMinNext << std::endl;
        std::cout << "iconDayNext: " << this->iconDayNext << std::endl;
        std::cout << "textDayNext: " << this->textDayNext << std::endl;
    } else {
        std::cerr << "Error: " << response.error.message << std::endl;
    }
    return response.status_code == 200;
}

bool QWeather::Weather::GetNextHourWeather() {
    // 使用config类中的属性，按照fmt::format格式化指定参数的格式，获取API地址
    std::string str24hWeatherUrl{
            fmt::format(QWeather::Weather::url24hWeather, std::string(this->config.GetLocationCode()),
                        std::string(this->config.GetQWeatherKey()))};
    //打印API地址
    std::cout << "24hWeatherUrl: " << str24hWeatherUrl << std::endl;
    // 创建一个cpr::Url对象
    auto cprUrl24hWeather{cpr::Url{str24hWeatherUrl}};
    // 创建一个GET请求
    auto response = cpr::Get(cprUrl24hWeather);
    // 检查请求是否成功
    if (response.status_code == 200) {
        // 输出响应内容
        std::cout << "Response: " << response.text << std::endl;
        // 将响应内容转换为json对象
        // 响应格式：{"code":"200","updateTime":"2023-12-19T20:35+08:00","fxLink":"https://www.qweather.com/weather/beijing-101010100.html","hourly":[{"fxTime":"2023-12-19T22:00+08:00","temp":"-9","icon":"150","text":"晴","wind360":"340","windDir":"北风","windScale":"1-3","windSpeed":"18","humidity":"29","pop":"6","precip":"0.0","pressure":"1034","cloud":"0","dew":"-24"},{"fxTime":"2023-12-19T23:00+08:00","temp":"-9","icon":"150","text":"晴","wind360":"339","windDir":"北风","windScale":"1-3","windSpeed":"16","humidity":"29","pop":"6","precip":"0.0","pressure":"1034","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T00:00+08:00","temp":"-10","icon":"150","text":"晴","wind360":"338","windDir":"北风","windScale":"1-3","windSpeed":"14","humidity":"31","pop":"4","precip":"0.0","pressure":"1034","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T01:00+08:00","temp":"-10","icon":"150","text":"晴","wind360":"335","windDir":"西北风","windScale":"1-3","windSpeed":"13","humidity":"32","pop":"0","precip":"0.0","pressure":"1035","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T02:00+08:00","temp":"-10","icon":"150","text":"晴","wind360":"332","windDir":"西北风","windScale":"1-3","windSpeed":"11","humidity":"33","pop":"0","precip":"0.0","pressure":"1035","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T03:00+08:00","temp":"-11","icon":"150","text":"晴","wind360":"336","windDir":"西北风","windScale":"1-3","windSpeed":"11","humidity":"35","pop":"0","precip":"0.0","pressure":"1034","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T04:00+08:00","temp":"-11","icon":"150","text":"晴","wind360":"340","windDir":"北风","windScale":"1-3","windSpeed":"9","humidity":"33","pop":"0","precip":"0.0","pressure":"1033","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T05:00+08:00","temp":"-12","icon":"150","text":"晴","wind360":"343","windDir":"北风","windScale":"1-3","windSpeed":"9","humidity":"33","pop":"0","precip":"0.0","pressure":"1032","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T06:00+08:00","temp":"-12","icon":"150","text":"晴","wind360":"347","windDir":"北风","windScale":"1-3","windSpeed":"9","humidity":"33","pop":"0","precip":"0.0","pressure":"1031","cloud":"0","dew":"-24"},{"fxTime":"2023-12-20T07:00+08:00","temp":"-12","icon":"150","text":"晴","wind360":"22","windDir":"北风","windScale":"1-3","windSpeed":"13","humidity":"32","pop":"0","precip":"0.0","pressure":"1031","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T08:00+08:00","temp":"-12","icon":"100","text":"晴","wind360":"26","windDir":"东北风","windScale":"1-3","windSpeed":"13","humidity":"30","pop":"0","precip":"0.0","pressure":"1031","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T09:00+08:00","temp":"-10","icon":"100","text":"晴","wind360":"4","windDir":"北风","windScale":"1-3","windSpeed":"9","humidity":"28","pop":"0","precip":"0.0","pressure":"1031","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T10:00+08:00","temp":"-9","icon":"100","text":"晴","wind360":"325","windDir":"西北风","windScale":"1-3","windSpeed":"7","humidity":"25","pop":"0","precip":"0.0","pressure":"1032","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T11:00+08:00","temp":"-7","icon":"100","text":"晴","wind360":"292","windDir":"西风","windScale":"1-3","windSpeed":"9","humidity":"22","pop":"0","precip":"0.0","pressure":"1033","cloud":"0","dew":"-26"},{"fxTime":"2023-12-20T12:00+08:00","temp":"-7","icon":"100","text":"晴","wind360":"275","windDir":"西风","windScale":"1-3","windSpeed":"14","humidity":"21","pop":"2","precip":"0.0","pressure":"1033","cloud":"0","dew":"-26"},{"fxTime":"2023-12-20T13:00+08:00","temp":"-6","icon":"100","text":"晴","wind360":"266","windDir":"西风","windScale":"1-3","windSpeed":"18","humidity":"20","pop":"6","precip":"0.0","pressure":"1033","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T14:00+08:00","temp":"-6","icon":"100","text":"晴","wind360":"261","windDir":"西风","windScale":"3-4","windSpeed":"24","humidity":"21","pop":"6","precip":"0.0","pressure":"1033","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T15:00+08:00","temp":"-6","icon":"100","text":"晴","wind360":"264","windDir":"西风","windScale":"3-4","windSpeed":"20","humidity":"21","pop":"6","precip":"0.0","pressure":"1034","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T16:00+08:00","temp":"-6","icon":"100","text":"晴","wind360":"268","windDir":"西风","windScale":"1-3","windSpeed":"18","humidity":"22","pop":"6","precip":"0.0","pressure":"1034","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T17:00+08:00","temp":"-7","icon":"150","text":"晴","wind360":"273","windDir":"西风","windScale":"1-3","windSpeed":"16","humidity":"25","pop":"6","precip":"0.0","pressure":"1034","cloud":"0","dew":"-25"},{"fxTime":"2023-12-20T18:00+08:00","temp":"-7","icon":"150","text":"晴","wind360":"279","windDir":"西风","windScale":"1-3","windSpeed":"13","humidity":"26","pop":"6","precip":"0.0","pressure":"1034","cloud":"0","dew":"-26"},{"fxTime":"2023-12-20T19:00+08:00","temp":"-8","icon":"150","text":"晴","wind360":"257","windDir":"西风","windScale":"1-3","windSpeed":"7","humidity":"26","pop":"7","precip":"0.0","pressure":"1035","cloud":"0","dew":"-26"},{"fxTime":"2023-12-20T20:00+08:00","temp":"-9","icon":"150","text":"晴","wind360":"270","windDir":"西风","windScale":"1-3","windSpeed":"5","humidity":"26","pop":"7","precip":"0.0","pressure":"1034","cloud":"0","dew":"-27"},{"fxTime":"2023-12-20T21:00+08:00","temp":"-11","icon":"150","text":"晴","wind360":"281","windDir":"西风","windScale":"1-3","windSpeed":"5","humidity":"27","pop":"7","precip":"0.0","pressure":"1035","cloud":"0","dew":"-27"}],"refer":{"sources":["QWeather"],"license":["CC BY-SA 4.0"]}}
        auto json24hResponse{nlohmann::json::parse(response.text).at("hourly").at(0)};
        std::cout << json24hResponse.dump(3) << std::endl;
        // 对json对象进行解析，将解析后的数据赋给类中对应的属性
        this->nextWeatherText = json24hResponse.at(0).at("text").get<std::string>();
        // 打印解析后的数据
        std::cout << "nextWeatherText: " << this->nextWeatherText << std::endl;
    } else {
        std::cerr << "Error: " << response.error.message << std::endl;
    }
    return response.status_code == 200;
}

bool QWeather::Weather::GetNowWeather() {
    // 使用config类中的属性，按照fmt::format格式化指定参数的格式，获取API地址
    std::string strNowWeatherUrl{
            fmt::format(QWeather::Weather::urlNowWeather, std::string(this->config.GetLocationCode()),
                        std::string(this->config.GetQWeatherKey()))};
    //打印API地址
    std::cout << "NowWeatherUrl: " << strNowWeatherUrl << std::endl;
    // 创建一个cpr::Url对象
    auto cprUrlNowWeather{cpr::Url{strNowWeatherUrl}};
    // 创建一个GET请求
    auto response = cpr::Get(cprUrlNowWeather);
    // 检查请求是否成功
    if (response.status_code == 200) {
        // 输出响应内容
        std::cout << "Response: " << response.text << std::endl;
        // 将响应内容转换为json对象
        // 响应格式：{"code":"200","updateTime":"2023-12-19T21:27+08:00","fxLink":"https://www.qweather.com/weather/beijing-101010100.html","now":{"obsTime":"2023-12-19T21:14+08:00","temp":"-10","feelsLike":"-17","icon":"150","text":"晴","wind360":"16","windDir":"北风","windScale":"3","windSpeed":"13","humidity":"28","precip":"0.0","pressure":"1037","vis":"30","cloud":"0","dew":"-24"},"refer":{"sources":["QWeather"],"license":["CC BY-SA 4.0"]}}
        auto jsonNowWeather{nlohmann::json::parse(response.text).at("now")};
        std::cout << "Response: " << jsonNowWeather.dump(3) << std::endl;
        // 对json对象进行解析，将解析后的数据赋给类中对应的属性
        // 将json中的字符串转为数字赋给类中对应的属性
        this->nowWeatherText = jsonNowWeather.at(0).at("text").get<std::string>();
        // 打印解析后的数据
        std::cout << "nowWeatherText: " << this->nowWeatherText << std::endl;
    } else {
        std::cerr << "Error: " << response.error.message << std::endl;
    }
    return response.status_code == 200;
}

int QWeather::Weather::GetTempMax() {
    // 判断是否是今天，如果不是调用一次GetTodayWeather
    if (!this->fxDate.IsToday()) {
        this->GetTodayWeather();
    }
    return this->tempMax;
}

int QWeather::Weather::GetTempMin() {
    // 判断是否是今天，如果不是调用一次GetTodayWeather
    if (!this->fxDate.IsToday()) {
        this->GetTodayWeather();
    }
    return this->tempMin;
}

int QWeather::Weather::GetIconDay() {
    // 判断是否是今天，如果不是调用一次GetTodayWeather
    if (!this->fxDate.IsToday()) {
        this->GetTodayWeather();
    }
    return this->iconDay;
}

std::string_view QWeather::Weather::GetTextDay() {
    // 判断是否是今天，如果不是调用一次GetTodayWeather
    if (!this->fxDate.IsToday()) {
        this->GetTodayWeather();
    }
    return this->textDay;
}

bool QWeather::Weather::IsRainingInOneHour() {
    if (!(this->GetNextHourWeather() && this->GetNowWeather())) {
        return false;
    }
    // 声明一个变量判断现在是否在下雨
    auto isNowRaining{this->nowWeatherText.find("雨") != std::string::npos};
    // 声明一个变量判断下一个小时是否在下雨
    auto isNextRaining{this->nextWeatherText.find("雨") != std::string::npos};
    return !isNowRaining && isNextRaining;
}

int QWeather::Weather::GetTempMaxNext() {
    return this->tempMaxNext;
}

int QWeather::Weather::GetTempMinNext() {
    return this->tempMinNext;
}

int QWeather::Weather::GetIconDayNext() {
    return this->iconDayNext;
}

std::string_view QWeather::Weather::GetTextDayNext() {
    return this->textDayNext;
}

