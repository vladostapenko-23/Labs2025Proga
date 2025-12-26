#include "FaceDetector.hpp"
#include "Logger.hpp"
#include "ConfigManager.hpp"
#include <chrono>

FaceDetector::FaceDetector() {
    Logger::getInstance().info("Initializing FaceDetector...");
    
    // Отримуємо параметри з конфіга
    float thresh = ConfigManager::getInstance().getThreshold();
    Logger::getInstance().info("Confidence threshold set to: " + std::to_string(thresh));

    workerThread = std::thread(&FaceDetector::workerLoop, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    cv.notify_one();
    if (workerThread.joinable()) workerThread.join();
}

void FaceDetector::loadModel(const std::string& m, const std::string& c) {
    // Тут ми ігноруємо аргументи, якщо хочемо брати суто з конфіга, 
    // або використовуємо їх як fallback.
    // Але для чистоти завдання візьмемо з ConfigManager прямо тут:
    
    std::string proto = ConfigManager::getInstance().getModelProto();
    std::string weights = ConfigManager::getInstance().getModelWeights();

    Logger::getInstance().info("Loading model params from ConfigManager...");
    Logger::getInstance().debug("Proto: " + proto);
    Logger::getInstance().debug("Weights: " + weights);

    try {
        net = cv::dnn::readNet(weights, proto);
        if (net.empty()) {
             Logger::getInstance().error("Failed to load network! Check paths in settings.json");
        } else {
             Logger::getInstance().info("Model loaded successfully.");
             net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
             net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
        }
    } catch (const cv::Exception& e) {
        Logger::getInstance().error("OpenCV Exception: " + std::string(e.what()));
    }
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

        try {
            cv::Mat blob = cv::dnn::blobFromImage(currentJob, 1.0, cv::Size(300, 300), cv::Scalar(104, 177, 123));
            net.setInput(blob);
            cv::Mat det = net.forward();

            std::vector<cv::Rect> detected;
            cv::Mat detM(det.size[2], det.size[3], CV_32F, det.ptr<float>());
            
            // Беремо поріг з конфіга
            float confThreshold = ConfigManager::getInstance().getThreshold();

            for (int i = 0; i < detM.rows; i++) {
                float confidence = detM.at<float>(i, 2);
                if (confidence > confThreshold) {
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
        } catch (const std::exception& e) {
            Logger::getInstance().error("Worker exception: " + std::string(e.what()));
        }
    }
}
