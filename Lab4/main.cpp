#include <opencv2/opencv.hpp>
#include "FaceDetector.hpp"
#include "Logger.hpp"
#include "ConfigManager.hpp"

int main() {
    // 1. Завантажуємо налаштування
    ConfigManager::getInstance().loadConfig("settings.json");

    // 2. Налаштовуємо логер на основі конфіга
    std::string logLevel = ConfigManager::getInstance().getLogLevel();
    if (logLevel == "DEBUG") Logger::getInstance().setLevel(LogLevel::DEBUG);
    else if (logLevel == "WARN") Logger::getInstance().setLevel(LogLevel::WARN);
    else if (logLevel == "ERROR") Logger::getInstance().setLevel(LogLevel::ERROR);
    else Logger::getInstance().setLevel(LogLevel::INFO);

    Logger::getInstance().info("HW8: JSON Config System started.");

    // 3. Створюємо детектор (він сам витягне шляхи до моделі з ConfigManager)
    FaceDetector detector;
    // Викликаємо loadModel з пустими рядками, бо тепер шляхи беруться всередині класу з конфіга
    detector.loadModel("", "");

    // 4. Відкриваємо камеру за ID з конфіга
    int camID = ConfigManager::getInstance().getCameraID();
    Logger::getInstance().info("Opening camera ID: " + std::to_string(camID));
    
    cv::VideoCapture cap(camID);
    if (!cap.isOpened()) {
        Logger::getInstance().error("Failed to open camera!");
        return -1;
    }

    // Встановлюємо роздільну здатність (якщо камера підтримує)
    int width = ConfigManager::getInstance().getFrameWidth();
    int height = ConfigManager::getInstance().getFrameHeight();
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    cv::Mat frame;
    while (cv::waitKey(1) != 27) {
        cap >> frame;
        if (frame.empty()) {
             Logger::getInstance().warn("Empty frame.");
             break;
        }

        detector.updateFrame(frame);
        auto faces = detector.getLatestResults();

        for (const auto& r : faces) {
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2);
            // Додамо підпис з порогом
            cv::putText(frame, "Conf > " + std::to_string(ConfigManager::getInstance().getThreshold()), 
                        cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("HW8: JSON Configuration", frame);
    }

    Logger::getInstance().info("Application finished.");
    return 0;
}
