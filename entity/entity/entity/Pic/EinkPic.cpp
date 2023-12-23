//
// Created by zeng on 23-12-19.
//

#include <opencv2/freetype.hpp>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "EinkPic.h"
#include "../Calendar/Calendar.h"
#include "fmt/format.h"

EinkPic::EinkPic::EinkPic(std::string_view jsonFilePath) : config(jsonFilePath), weather(config) {
    this->image = cv::Mat(config.GetHeight(), config.GetWidth(), CV_8UC1);
    // cv::cvtColor(image, this->image, cv::COLOR_BGR2GRAY);  
    this->image.setTo(cv::Scalar(255));
}

int
EinkPic::EinkPic::AppendFontToImg(const std::string_view text, const cv::Scalar &color, const std::string_view ttfPath,
                                  const int fontHeight, const int x, const int y) {
    if (this->image.cols <= x || this->image.rows <= y) {
        std::cout << "图片大小不够" << std::endl;
        return -1;
    }
//    cv::Ptr<cv::freetype::FreeType2> ft2;
    cv::Mat colorImage;
    cv::cvtColor(this->image, colorImage, cv::COLOR_GRAY2BGR);
    auto ft2{cv::freetype::createFreeType2()};
    // 加载一个ttf字体文件，这里使用了一个名为simhei.ttf的黑体字
    ft2->loadFontData(ttfPath.data(), 0);
    auto org{cv::Point(x, y + fontHeight)}; // 文字的左下角坐标
    ft2->putText(colorImage, text.data(), org, fontHeight, color, -1, 8, true);
    cv::cvtColor(colorImage, this->image, cv::COLOR_BGR2GRAY);
    return y + fontHeight + 1;
}

void EinkPic::EinkPic::transparentToWhite(cv::Mat &img) {
    // 检查图片是否有四个通道（RGBA）
    if (img.channels() == 4) {
        // 创建一个白色的图片，大小和输入图片相同
        auto white{cv::Mat(img.size(), CV_8UC3, cv::Scalar(255, 255, 255))};
        // 创建一个临时的图片，用于存储BGR颜色空间的img
        cv::Mat temp;
        // 将img的颜色空间从BGRA转换为BGR
        cv::cvtColor(img, temp, cv::COLOR_BGRA2BGR);
        // 遍历图片的每个像素
        for (int y = 0; y < img.rows; ++y) {
            for (int x = 0; x < img.cols; ++x) {
                // 获取当前像素的透明度（alpha值）
                auto alpha{img.at<cv::Vec4b>(y, x)[3]};
                // 如果透明度不为0，说明该像素不透明
                if (alpha != 0) {
                    // 将该像素的BGR值复制到白色图片的对应位置
                    white.at<cv::Vec3b>(y, x) = temp.at<cv::Vec3b>(y, x);
                }
            }
        }
        // 将白色图片的内容赋值给输入图片
        img = white;
    }
}

int EinkPic::EinkPic::AppendPicToImg(std::string_view picPath, const int x, const int y, const double scaleFactor) {
    // 读取图片
    if (this->image.cols <= x || this->image.rows <= y) {
        std::cout << "图片大小不够" << std::endl;
        return -1;
    }

    cv::Mat pic = cv::imread(picPath.data(), cv::IMREAD_UNCHANGED);
    if (pic.empty()) {
        std::cout << "无法打开或找到图片" << std::endl;
        return -1;
    }
    // 如果图片有四个通道（RGBA），则将透明背景转换为白色背景
    if (pic.channels() == 4) {
        transparentToWhite(pic);
    }
    // 创建一个新的Mat对象来存储调整大小后的图片
    cv::Mat resizedPic = pic;
    if (scaleFactor != 1) {    // 调整图片大小
        cv::resize(pic, resizedPic, cv::Size(), scaleFactor, scaleFactor, cv::INTER_LINEAR);
    }
    cv::cvtColor(resizedPic, pic, cv::COLOR_BGR2GRAY);
    pic.convertTo(resizedPic, CV_8UC1);
    for (int i = 0; i < resizedPic.rows; i++) {
        for (int j = 0; j < resizedPic.cols; j++) {
            this->image.at<uchar>(i + y, j + x) = resizedPic.at<uchar>(i, j);
        }
    }
    return y + resizedPic.rows + 1;
}

void EinkPic::EinkPic::AppendCalendarToImg(std::string_view fontPath) {
    Calendar::Calendar calendar(fontPath, this->config.GetWidth(), this->config.GetHeight() * 2 / 3);
    // 调用函数，生成日历图像
    calendar.generateCalendar(this->image, 0, this->config.GetHeight() - calendar.GetHeight() - 1);
}

void EinkPic::EinkPic::ShowImg() const {
    cv::imshow("image", this->image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

const cv::Mat &EinkPic::EinkPic::GetImg() const {
    return this->image;
}

void EinkPic::EinkPic::InitImg() {
    this->AppendCalendarToImg(this->config.GetMengMeiZiTiPath());
    auto color{cv::Scalar(0)}; // 黑色
    auto picPath{fmt::format("{}/{}.png", this->config.GetPicPath(), this->weather.GetIconDay())};
    this->AppendPicToImg(picPath, 0, 0, 1);
    auto text{fmt::format("{}  {}~~{}°C", this->weather.GetTextDay(), this->weather.GetTempMax(),
                          this->weather.GetTempMin())};
    this->AppendFontToImg(text, color, this->config.GetSmileySansPath(), 30, 15, 120);
    auto picPathNext{fmt::format("{}/{}.png", this->config.GetPicPath(), this->weather.GetIconDayNext())};
    this->AppendPicToImg(picPathNext, 0 + 170, 0, 1);
    auto textNext{fmt::format("{}  {}~~{}°C", this->weather.GetTextDayNext(), this->weather.GetTempMaxNext(),
                          this->weather.GetTempMinNext())};
    this->AppendFontToImg(textNext, color, this->config.GetSmileySansPath(), 30, 15 + 170, 120);
}
