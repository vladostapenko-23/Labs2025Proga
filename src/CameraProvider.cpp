#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int deviceID) {
    cap.open(deviceID);
}

bool CameraProvider::isOpened() const {
    return cap.isOpened();
}

cv::Mat CameraProvider::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}
