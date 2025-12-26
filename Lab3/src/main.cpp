#include <opencv2/opencv.hpp>
#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include <chrono>

int main() {
    CameraProvider camera(0);
    if (!camera.isOpened()) {
        std::cerr << "Error: Could not open camera!" << std::endl;
        return -1;
    }

    KeyProcessor keyProc;
    FrameProcessor frameProc;
    Display display("Video Studio");

    int brightness = 50;
    cv::createTrackbar("Brightness", "Video Studio", &brightness, 100);

    auto lastTime = std::chrono::high_resolution_clock::now();
    double fps = 0;

    while (true) {
        cv::Mat frame = camera.getFrame();
        if (frame.empty()) break;

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        fps = 1.0 / elapsed.count();
        lastTime = currentTime;

        int key = cv::waitKey(1);
        if (key == 27) break; // ESC

        keyProc.process(key);
        frameProc.process(frame, keyProc.getMode(), brightness);
        frameProc.addOverlay(frame, keyProc.getMode(), fps);

        display.show(frame);
    }

    cv::destroyAllWindows();
    return 0;
}
