//
// Created by zeng on 23-12-17.
//

#include <opencv2/freetype.hpp>
#include "Calendar.h"
#include "fmt/format.h"

Calendar::Calendar::Calendar(std::string_view fontPath, int width, int height)
        : fontPath{fontPath}, width{width}, height{height} {
    // 设置日期和时间
    time_t now = time(0);
    tm *ltm = localtime(&now);
    // 获取当前月份和年份
    this->year = 1900 + ltm->tm_year;
    this->month = 1 + ltm->tm_mon;
    this->day = ltm->tm_mday;
}

int Calendar::Calendar::getWeekday(int year, int month, int day) {
    if (month == 1 || month == 2) {
        year--;
        month += 12;
    }
    int c = year / 100;
    int y = year % 100;
    int m = month;
    int w = (c / 4 - 2 * c + y + y / 4 + 13 * (m + 1) / 5 + day - 1) % 7;
    if (w < 0) w += 7;
    return w;
}

int Calendar::Calendar::getDays() {
    // 定义一个数组，存储每个月的天数
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // 如果是闰年，2月有29天
    if (this->year % 4 == 0 && (this->year % 100 != 0 || this->year % 400 == 0)) {
        days[1] = 29;
    }
    // 返回该月的天数
    return days[this->month - 1];
}

int Calendar::Calendar::getRows() {
    // 获取该月的第一天是星期几
    int firstWeekday = getWeekday(this->year, this->month, 1);
    // 获取该月的天数
    int days = getDays();
    // 计算该月的日期区的行数
    int rows = (firstWeekday + days - 1) / 7 + 1;
    // 返回行数
    return rows;
}

void
Calendar::Calendar::generateCalendar(cv::Mat &image, const int x,
                                     const int y) {
    // 加载ttf字体文件
    // cv::Ptr<cv::freetype::FreeType2> ft2;
    auto ft2{cv::freetype::createFreeType2()};
    ft2->loadFontData(this->fontPath, 0);

//    // 创建一个空白的白色图像
//    cv::Mat image = cv::Mat::ones(height, width, CV_8UC1) * 255;
    // 定义字体的颜色
    auto fontColor{cv::Scalar(0, 0, 0)};
    // 定义一个数组，存储星期的名称
    auto weekdays{std::array<std::string, 7>{"日", "一", "二", "三", "四", "五", "六"}};
    // 定义一个变量，存储当前月份的名称
    auto months{
            std::array<std::string, 12>{"一月", "二月", "三月", "四月", "五月", "六月", "七月", "八月", "九月", "十月",
                                        "十一月",
                                        "十二月"}};
    auto monthName{months[this->month - 1]};
    // 获取该月的日期区的行数
    auto rows{this->getRows()};
//    // 根据行数调整字体的大小
    auto fontHeight{this->height / (rows + 2)};
    auto singleWith{this->width / 7};
//    auto title{std::to_string(this->year) + "年" + monthName};
    auto title{fmt::format("{}年{}", this->year, monthName)};
    cv::Mat colorImage;
    cv::cvtColor(image, colorImage, cv::COLOR_GRAY2RGB);
    ft2->putText(colorImage, title, cv::Point((this->width - fontHeight / 3 * title.length() + x) / 2, fontHeight + y),
                 fontHeight,
                 fontColor,
                 -1, 8, true);
    // 在图像的第二行写上星期的名称
    for (int i = 0; i < 7; i++) {
        this->drawABlock(colorImage, ft2, weekdays[i], fontHeight, singleWith, i * singleWith + x,
                         2 * fontHeight + y, true);
    }
    // 获取该月的第一天是星期几
    auto firstWeekday{getWeekday(this->year, this->month, 1)};
    // 获取该月的天数
    auto days{this->getDays()};
    // 定义一个变量，记录当前的行数
    auto row{2};
    // 遍历该月的每一天
    for (int i = 1; i <= days; i++) {
        // 计算该天在图像中的位置
        int dayX = (firstWeekday + i - 1) % 7 * singleWith;
        int dayY = fontHeight + row * fontHeight;
        if (i == this->day) {
            this->drawABlock(colorImage, ft2, std::to_string(i), fontHeight, singleWith, x + dayX,
                             y + dayY, true);

        } else {
            // 在图像中写上该天的日期
            this->drawABlock(colorImage, ft2, std::to_string(i), fontHeight, singleWith, x + dayX,
                             y + dayY, false);
        }
        // 如果该天是星期六，换行
        if ((firstWeekday + i) % 7 == 0) {
            row++;
        }
    }
//    image = colorImage;
    cv::cvtColor(colorImage, image, cv::COLOR_RGB2GRAY);
}

void
Calendar::Calendar::drawABlock(cv::Mat &image, const cv::Ptr<cv::freetype::FreeType2> &ft2, const std::string &text,
                               const int fontHeight,
                               const int width, const int x, const int y, bool isBlack) {
    for (int imageY = 0; imageY < fontHeight; ++imageY) {
        for (int imageX = 0; imageX < width; ++imageX) {
            if (isBlack) {
                image.at<cv::Vec3b>(y - imageY, imageX + x) = cv::Vec3b(0, 0, 0); // Set pixel to black (BGR: 0, 0, 0)
            } else {
                image.at<cv::Vec3b>(y - imageY, imageX + x) = cv::Vec3b(255, 255, 255); // Set pixel to white (BGR: 255, 255, 255)
            }
        }
    }
    auto fontColor{isBlack ? cv::Vec3b(255, 255, 255) : cv::Vec3b(0, 0, 0)};
    ft2->putText(image, text, cv::Point(x + fontHeight / 2, y), fontHeight, fontColor,
                 -1, 8, true);
}

int Calendar::Calendar::GetWidth() {
    return this->width;
}

int Calendar::Calendar::GetHeight() {
    return this->height;
}
