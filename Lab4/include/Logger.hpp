#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

enum class LogLevel { INFO, WARN, ERROR };

class Logger {
public:
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&time);

        std::stringstream ss;
        ss << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] ";
        ss << "[" << levelToString(level) << "] " << message;
        
        std::string finalLog = ss.str();
        std::cout << finalLog << std::endl;
        if (logFile.is_open()) {
            logFile << finalLog << std::endl;
            logFile.flush();
        }
    }

    void info(const std::string& msg) { log(LogLevel::INFO, msg); }
    void warn(const std::string& msg) { log(LogLevel::WARN, msg); }
    void error(const std::string& msg) { log(LogLevel::ERROR, msg); }

private:
    Logger() {
        logFile.open("application.log", std::ios::app);
    }
    ~Logger() {
        if (logFile.is_open()) logFile.close();
    }

    std::string levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO ";
            case LogLevel::WARN: return "WARN ";
            case LogLevel::ERROR: return "ERROR";
            default: return "LOG  ";
        }
    }

    std::mutex logMutex;
    std::ofstream logFile;
};
