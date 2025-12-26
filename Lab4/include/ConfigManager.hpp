#pragma once
#include "json.hpp"
#include "Logger.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;

class ConfigManager {
public:
    // Singleton
    ConfigManager(const ConfigManager&) = delete;
    void operator=(const ConfigManager&) = delete;

    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Завантаження файлу
    void loadConfig(const std::string& path) {
        try {
            std::ifstream file(path);
            if (!file.is_open()) {
                Logger::getInstance().warn("Config file not found: " + path + ". Using defaults.");
                return;
            }
            config = json::parse(file);
            Logger::getInstance().info("Configuration loaded successfully from " + path);
        } catch (const std::exception& e) {
            Logger::getInstance().error("Error parsing config: " + std::string(e.what()) + ". Using defaults.");
        }
    }

    // --- Методи доступу з дефолтними значеннями ---

    int getCameraID() {
        // Якщо поле існує - беремо його, інакше - повертаємо 0
        return config["camera"].value("id", 0);
    }
    
    int getFrameWidth() { return config["camera"].value("width", 640); }
    int getFrameHeight() { return config["camera"].value("height", 480); }

    std::string getModelProto() {
        return config["model"].value("proto", "models/deploy.prototxt");
    }

    std::string getModelWeights() {
        return config["model"].value("weights", "models/res10_300x300_ssd_iter_140000.caffemodel");
    }

    float getThreshold() {
        return config["model"].value("threshold", 0.5f);
    }

    std::string getLogLevel() {
        return config["logger"].value("level", "INFO");
    }

private:
    ConfigManager() = default;
    json config; // Тут зберігається весь JSON об'єкт
};
