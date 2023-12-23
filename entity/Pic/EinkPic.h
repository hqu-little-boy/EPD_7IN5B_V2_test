//
// Created by zeng on 23-12-19.
//

#ifndef CPR_TEST_EINKPIC_H
#define CPR_TEST_EINKPIC_H

#include <opencv2/core/mat.hpp>
#include "Config.h"
#include "../Qweather/Weather.h"
#include "../Sentence/Sentence.h"

namespace EinkPic {
    class EinkPic {
    public:
        EinkPic(std::string_view jsonFilePath);

        int AppendFontToImg(std::string_view text, const cv::Scalar &color, std::string_view ttfPath,
                            int fontHeight, int x, int y);

        int AppendPicToImg(std::string_view picPath, const int x, const int y, const double scaleFactor);

        void AppendCalendarToImg(std::string_view fontPath);

        void ShowImg() const;

        void InitImg();

        const cv::Mat &GetImg() const;

    private:
        void transparentToWhite(cv::Mat &img);

        cv::Mat image;
        Config config;
        QWeather::Weather weather;
//        Sentence::Sentence sentence;
    };
}

#endif //CPR_TEST_EINKPIC_H
