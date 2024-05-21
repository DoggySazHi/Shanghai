#include "ShanghaiConfiguration.h"

#include <fstream>

ShanghaiConfiguration* ShanghaiConfiguration::instance = nullptr;

ShanghaiConfiguration *ShanghaiConfiguration::getInstance() {
    if (instance == nullptr) {
        instance = new ShanghaiConfiguration();
    }

    return instance;
}

ShanghaiConfiguration::ShanghaiConfiguration() {
    std::ifstream file(CONFIG_FILE);
    if (file.is_open()) {
        Json::Value root;
        file >> root;
        deserialize(root);
    } else {
        // Write default values
        std::ofstream ofile(CONFIG_FILE);
        Json::Value root;
        serialize(root);
        ofile << root;
        ofile.close();
    }
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
