//
// Created by zeng on 23-12-17.
//

#ifndef OPENCV_TEST_CALENDAR_H
#define OPENCV_TEST_CALENDAR_H


#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
namespace Calendar {
    class Calendar {
    public:
        Calendar(std::string_view fontPath, int width, int height);

        // 定义一个函数，根据年份和月份生成一个黑白日历图像
        void generateCalendar(cv::Mat &image, int x, int y);
        //获取width
        int GetWidth();
        //获取width
        int GetHeight();
    private:
        // 定义一个函数，根据基姆拉尔森公式计算某年某月某日是星期几
        // 公式为：W = (d + 2*m + 3*(m+1)/5 + y + y/4 - y/100 + y/400) mod 7
        // 其中，y是年份，m是月份，d是日期，W是星期几（0为星期日，1为星期一，以此类推）
        int getWeekday(int year, int month, int day);

        // 定义一个函数，根据年份和月份返回该月的天数
        int getDays();

        // 定义一个函数，根据年份和月份返回该月的日期区的行数
        int getRows();

        void
        drawABlock(cv::Mat &image, const cv::Ptr<cv::freetype::FreeType2> &ft2, const std::string &text, int fontHeight,
                   int width, int x, int y, bool isBlack);

        int year;
        int month;
        int day;
        int width;
        int height;
        std::string fontPath;
    };
}

#endif //OPENCV_TEST_CALENDAR_H
