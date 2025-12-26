cmake_minimum_required(VERSION 3.10)
project(HW8_JSON)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
include_directories(include)

# Додаємо всі source файли
add_executable(app main.cpp src/FaceDetector.cpp)
target_link_libraries(app ${OpenCV_LIBS} Threads::Threads)
