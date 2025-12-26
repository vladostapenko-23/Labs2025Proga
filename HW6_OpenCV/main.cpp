#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Відкриваємо камеру (0 - дефолтна камера)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera" << std::endl;
        return -1;
    }

    cv::Mat frame;
    std::cout << "Press 'ESC' or 'q' to exit..." << std::endl;

    while (true) {
        cap >> frame; // Зчитуємо кадр
        if (frame.empty()) break;

        // --- Малювання (Вимога завдання) ---
        
        // 1. Малюємо прямокутник по центру
        cv::Point center(frame.cols / 2, frame.rows / 2);
        cv::rectangle(frame, 
                      cv::Point(center.x - 100, center.y - 100), 
                      cv::Point(center.x + 100, center.y + 100), 
                      cv::Scalar(0, 255, 0), // Зелений колір
                      3);

        // 2. Додаємо текст
        cv::putText(frame, 
                    "HW6: OpenCV Test", 
                    cv::Point(20, 50), 
                    cv::FONT_HERSHEY_SIMPLEX, 
                    1.0, 
                    cv::Scalar(0, 0, 255), // Червоний колір
                    2);

        // Виводимо результат
        cv::imshow("HW6 Camera", frame);

        // Чекаємо клавішу (1 мс). Якщо ESC (27) або 'q' - вихід
        char c = (char)cv::waitKey(1);
        if (c == 27 || c == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
