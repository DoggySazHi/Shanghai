#include "ShanghaiConfiguration.h"

#include <filesystem>
#include <fstream>

std::string ShanghaiConfiguration::getConfigPath() {
    auto configHome = std::getenv("XDG_CONFIG_HOME");
    if (configHome == nullptr) {
        configHome = std::getenv("HOME");
        if (configHome == nullptr) {
            return "";
        }

        return std::string(configHome) + "/.config/shanghai/";
    }

    return std::string(configHome) + "/shanghai/";
}

ShanghaiConfiguration* ShanghaiConfiguration::instance = nullptr;

ShanghaiConfiguration *ShanghaiConfiguration::getInstance() {
    if (instance == nullptr) {
        instance = new ShanghaiConfiguration();
    }

    return instance;
}

ShanghaiConfiguration::ShanghaiConfiguration() {
    auto configPath = getConfigPath();
    std::filesystem::create_directories(configPath);
    configPath += "shanghai.json";

    std::ifstream file(configPath);

    if (file.is_open()) {
        Json::Value root;
        file >> root;
        deserialize(root);
    }

    save(configPath);
}

void ShanghaiConfiguration::save(const std::string& configPath) {
    std::ofstream ofile(configPath);
    Json::Value root;
    serialize(root);
    ofile << root;
    ofile.close();
}

ShanghaiConfiguration::~ShanghaiConfiguration() {
    delete instance;
}

void ShanghaiConfiguration::serialize(Json::Value &root) const {
    root["output"] = output;
    root["enableBackground"] = enableBackground;
    root["layer"] = layer;
    root["fallingPortalProbability"] = fallingPortalProbability;
}

void ShanghaiConfiguration::deserialize(const Json::Value &root) {
    output = root.get("output", UINT32_MAX).asUInt();
    enableBackground = root.get("enableBackground", false).asBool();
    layer = root.get("layer", 1).asUInt();
    fallingPortalProbability = root.get("fallingPortalProbability", 0.0f).asFloat();
}
