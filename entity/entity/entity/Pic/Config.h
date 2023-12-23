//
// Created by zeng on 23-12-18.
//

#ifndef CPR_TEST_CONFIG_H
#define CPR_TEST_CONFIG_H

#include <string>
#include "nlohmann/json.hpp"
//对应一个这个项目的名词空间，避免与第三方库冲突
namespace EinkPic {
    class Config {

    public:
        Config(std::string_view jsonFilePath);
        //获取width
        int GetWidth() const;
        //获取height
        int GetHeight() const;
        //获取location
        std::string_view GetLocation() const;

        //获取adm
        std::string_view GetAdm() const;

        //获取q-weather-key
        std::string_view GetQWeatherKey() const;

        //获取locationCode
        std::string_view GetLocationCode() const;

        //获取picPath
        std::string_view GetPicPath() const;

        //获取萌妹ttfPath
        std::string_view GetMengMeiZiTiPath() const;

        //获取SmileySansPath
        std::string_view GetSmileySansPath() const;
        // API地址
        static constexpr std::string_view urlLocation = "https://geoapi.qweather.com/v2/city/lookup?location={}&adm={}&key={}";

    private:
        int width;
        int height;
        std::string location;
        std::string adm;
        std::string q_weather_key;
        std::string picPath;
        std::string mengMeiZiTiPath;
        std::string smileySansPath;
        std::string locationCode;//城市代码
    };
}


#endif //CPR_TEST_CONFIG_H
