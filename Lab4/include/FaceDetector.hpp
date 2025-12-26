#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

class FaceDetector {
public:
    FaceDetector();
    ~FaceDetector();

    void loadModel(const std::string& modelPath, const std::string& configPath);
    void updateFrame(const cv::Mat& frame); // Передача кадру в потік
    std::vector<cv::Rect> getLatestResults(); // Отримання координат для UI

private:
    void workerLoop(); // Функція фонового потоку

    cv::dnn::Net net;
    std::thread workerThread;
    std::mutex dataMutex;
    std::condition_variable cv;

    std::atomic<bool> running{true};
    bool hasNewFrame{false};

    cv::Mat frameToProcess;
    std::vector<cv::Rect> faces;
};
