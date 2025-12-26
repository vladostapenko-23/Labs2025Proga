#include "Display.hpp"
#include <iostream>

Display::Display(const std::string& name) {
    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    cv::setMouseCallback(name, onMouse, nullptr);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) {
        cv::imshow("Video Studio", frame);
    }
}

void Display::onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        std::cout << "Mouse Click at: [" << x << ", " << y << "]" << std::endl;
    }
}
