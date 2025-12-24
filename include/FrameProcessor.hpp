#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    void process(cv::Mat& frame, Mode mode, int brightness);
    void addOverlay(cv::Mat& frame, Mode mode, double fps);
};
