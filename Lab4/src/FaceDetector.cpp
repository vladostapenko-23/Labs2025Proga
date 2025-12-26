#include "FaceDetector.hpp"
#include <chrono>

FaceDetector::FaceDetector() {
    workerThread = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    cv.notify_one();
    if (workerThread.joinable()) workerThread.join();
}

void FaceDetector::loadModel(const std::string& m, const std::string& c) {
    net = cv::dnn::readNet(m, c);
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(dataMutex);
    frameToProcess = frame.clone();
    hasNewFrame = true;
    cv.notify_one();
}

std::vector<cv::Rect> FaceDetector::getLatestResults() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return faces;
}

void FaceDetector::workerLoop() {
    while (running) {
        cv::Mat currentJob;
        {
            std::unique_lock<std::mutex> lock(dataMutex);
            cv.wait(lock, [this] { return hasNewFrame || !running; });
            if (!running) break;
            currentJob = frameToProcess.clone();
            hasNewFrame = false;
        }

        if (currentJob.empty()) continue;

        // Інференс нейромережі
        cv::Mat blob = cv::dnn::blobFromImage(currentJob, 1.0, cv::Size(300, 300), cv::Scalar(104, 177, 123));
        net.setInput(blob);
        cv::Mat det = net.forward();

        // Емуляція важкого навантаження (щоб показати плавність UI)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::vector<cv::Rect> detected;
        cv::Mat detM(det.size[2], det.size[3], CV_32F, det.ptr<float>());
        for (int i = 0; i < detM.rows; i++) {
            if (detM.at<float>(i, 2) > 0.5) {
                int x1 = detM.at<float>(i, 3) * currentJob.cols;
                int y1 = detM.at<float>(i, 4) * currentJob.rows;
                int x2 = detM.at<float>(i, 5) * currentJob.cols;
                int y2 = detM.at<float>(i, 6) * currentJob.rows;
                detected.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
            }
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            faces = detected;
        }
    }
}
