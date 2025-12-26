#include <opencv2/opencv.hpp>
#include "FaceDetector.hpp"
#include "Logger.hpp"

int main() {
    Logger::getInstance().info("HW7: Application started (Singleton Logger).");

    FaceDetector detector;
    // Шляхи до моделей
    detector.loadModel("models/res10_300x300_ssd_iter_140000.caffemodel", "models/deploy.prototxt");

    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        Logger::getInstance().error("Failed to open camera!");
        return -1;
    }
    Logger::getInstance().info("Camera opened successfully.");

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            Logger::getInstance().warn("Received empty frame.");
            break;
        }

        detector.updateFrame(frame);
        auto faces = detector.getLatestResults();

        for (const auto& r : faces) {
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("HW7: Logger Singleton", frame);
        if (cv::waitKey(1) == 27) break; // ESC
    }

    Logger::getInstance().info("Application finished.");
    return 0;
}
