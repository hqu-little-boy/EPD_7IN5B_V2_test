//
// Created by zeng on 23-12-18.
//

#include <fstream>
#include <iostream>
#include "Config.h"
#include "cpr/cprtypes.h"
#include "cpr/api.h"
#include "fmt/format.h"

EinkPic::Config::Config(std::string_view jsonFilePath) {
    //打开jsonFilePath文件，以json格式读取，将赋给类中对应的属性
    auto jsonFile{std::ifstream(jsonFilePath.data())};
    if (!jsonFile.is_open()) {
        std::cerr << "Error: " << "Can't open file: " << jsonFilePath << std::endl;
        exit(1);
    }
    nlohmann::json jsonConfig;
    jsonFile >> jsonConfig;
    this->width = jsonConfig["width"].get<int>();
    this->height = jsonConfig["height"].get<int>();
    this->location = jsonConfig["location"].get<std::string>();
    this->adm = jsonConfig["adm"].get<std::string>();
    this->q_weather_key = jsonConfig["q-weather-key"].get<std::string>();
    this->picPath = jsonConfig["picPath"].get<std::string>();
    this->mengMeiZiTiPath = jsonConfig["MengMeiZiTiPath"].get<std::string>();
    this->smileySansPath = jsonConfig["SmileySansPath"].get<std::string>();
    //使用config类中的属性，按照fmt::format格式化指定参数的格式，获取API地址
    auto urlLocation{
            cpr::Url{fmt::format(EinkPic::Config::urlLocation, this->location, this->adm, this->q_weather_key)}};
    // 创建一个GET请求
    auto response = cpr::Get(urlLocation);
    // 检查请求是否成功
    if (response.status_code == 200) {
        // 输出响应内容
        //回复格式
        //{"code":"200","location":[{"name":"北京","id":"101010100","lat":"39.90499","lon":"116.40529","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"10","fxLink":"https://www.qweather.com/weather/beijing-101010100.html"},{"name":"海淀","id":"101010200","lat":"39.95607","lon":"116.31032","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"15","fxLink":"https://www.qweather.com/weather/haidian-101010200.html"},{"name":"朝阳","id":"101010300","lat":"39.92149","lon":"116.48641","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"15","fxLink":"https://www.qweather.com/weather/chaoyang-101010300.html"},{"name":"顺义","id":"101010400","lat":"40.12894","lon":"116.65353","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"33","fxLink":"https://www.qweather.com/weather/shunyi-101010400.html"},{"name":"怀柔","id":"101010500","lat":"40.32427","lon":"116.63712","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"33","fxLink":"https://www.qweather.com/weather/huairou-101010500.html"},{"name":"通州","id":"101010600","lat":"39.90249","lon":"116.65860","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"23","fxLink":"https://www.qweather.com/weather/tongzhou-101010600.html"},{"name":"昌平","id":"101010700","lat":"40.21809","lon":"116.23591","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"23","fxLink":"https://www.qweather.com/weather/changping-101010700.html"},{"name":"延庆","id":"101010800","lat":"40.46532","lon":"115.98501","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"33","fxLink":"https://www.qweather.com/weather/yanqing-101010800.html"},{"name":"丰台","id":"101010900","lat":"39.86364","lon":"116.28696","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"25","fxLink":"https://www.qweather.com/weather/fengtai-101010900.html"},{"name":"石景山","id":"101011000","lat":"39.91460","lon":"116.19544","adm2":"北京","adm1":"北京市","country":"中国","tz":"Asia/Shanghai","utcOffset":"+08:00","isDst":"0","type":"city","rank":"35","fxLink":"https://www.qweather.com/weather/shijingshan-101011000.html"}],"refer":{"sources":["QWeather"],"license":["QWeather Developers License"]}}
        std::cout << "Response: " << response.text << std::endl;
        auto jsonLocationId{nlohmann::json::parse(response.text).at("location").at(0)};
        std::cout << "Response: " << jsonLocationId << std::endl;
        // 获取locationCode
        this->locationCode = jsonLocationId.at(0).at("id").get<std::string_view>();
        std::cout << "locationCode: " << this->locationCode << std::endl;
    } else {
        std::cerr << "Error: " << response.error.message << std::endl;
    }
}

std::string_view EinkPic::Config::GetLocation() const {
    return this->location;
}

std::string_view EinkPic::Config::GetAdm() const {
    return this->adm;
}

std::string_view EinkPic::Config::GetQWeatherKey() const {
    return this->q_weather_key;
}

std::string_view EinkPic::Config::GetLocationCode() const {
    return this->locationCode;
}

std::string_view EinkPic::Config::GetPicPath() const {
    return this->picPath;
}

std::string_view EinkPic::Config::GetMengMeiZiTiPath() const {
    return this->mengMeiZiTiPath;
}

std::string_view EinkPic::Config::GetSmileySansPath() const {
    return this->smileySansPath;
}

int EinkPic::Config::GetWidth() const {
    return this->width;
}

int EinkPic::Config::GetHeight() const {
    return this->height;
}
