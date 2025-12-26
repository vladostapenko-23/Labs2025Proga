#include <opencv2/opencv.hpp>
#include "FaceDetector.hpp"

int main() {
    FaceDetector detector;
    detector.loadModel("models/res10_300x300_ssd_iter_140000.caffemodel", "models/deploy.prototxt");

    cv::VideoCapture cap(0);
    cv::Mat frame;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // Асинхронно відправляємо кадр на детекцію
        detector.updateFrame(frame);

        // UI потік не чекає! Він одразу забирає останні відомі результати
        auto faces = detector.getLatestResults();

        for (const auto& r : faces) {
            cv::rectangle(frame, r, cv::Scalar(0, 255, 0), 2);
            cv::putText(frame, "Face", r.tl(), 1, 1.2, cv::Scalar(0, 255, 0), 2);
        }

        cv::imshow("Lab 4: Async DNN (Level 2)", frame);
        if (cv::waitKey(1) == 27) break;
    }
    return 0;
}
