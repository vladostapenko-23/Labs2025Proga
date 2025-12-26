#include "FrameProcessor.hpp"

void FrameProcessor::process(cv::Mat& frame, Mode mode, int brightness) {
    if (frame.empty()) return;

    // Слайдер яскравості
    frame.convertTo(frame, -1, 1, brightness - 50);

    switch(mode) {
        case Mode::CANNY: {
            cv::Mat gray, edges;
            cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
            cv::Canny(gray, edges, 100, 200);
            cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
            break;
        }
        case Mode::BLUR:
            cv::GaussianBlur(frame, frame, cv::Size(25, 25), 0);
            break;
        case Mode::INVERT:
            cv::bitwise_not(frame, frame);
            break;
        case Mode::GLITCH: {
            std::vector<cv::Mat> channels;
            cv::split(frame, channels);
            // Зсув червоного каналу
            if (frame.cols > 20) {
                cv::Mat shiftedR = cv::Mat::zeros(frame.size(), channels[2].type());
                channels[2](cv::Rect(15, 0, frame.cols-15, frame.rows)).copyTo(shiftedR(cv::Rect(0, 0, frame.cols-15, frame.rows)));
                channels[2] = shiftedR;
            }
            cv::merge(channels, frame);
            break;
        }
        default: break;
    }
}

void FrameProcessor::addOverlay(cv::Mat& frame, Mode mode, double fps) {
    std::string text = "FPS: " + std::to_string((int)fps) + " | Mode: " + std::to_string((int)mode + 1);
    cv::putText(frame, text, {20, 40}, cv::FONT_HERSHEY_SIMPLEX, 0.8, {0, 255, 0}, 2);
    cv::putText(frame, "Keys: 1-Normal, 2-Canny, 3-Blur, 4-Invert, 5-Glitch", {20, frame.rows - 20}, 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, {255, 255, 255}, 1);
}
