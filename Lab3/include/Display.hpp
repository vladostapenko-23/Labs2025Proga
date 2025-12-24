#pragma once
#include <opencv2/opencv.hpp>

class Display {
public:
    Display(const std::string& name);
    void show(const cv::Mat& frame);
    static void onMouse(int event, int x, int y, int flags, void* userdata);
};
