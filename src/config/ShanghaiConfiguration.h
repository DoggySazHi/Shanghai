#ifndef SHANGHAI_SHANGHAICONFIGURATION_H
#define SHANGHAI_SHANGHAICONFIGURATION_H

#include <json/json.h>


class ShanghaiConfiguration {
public:
    static ShanghaiConfiguration* getInstance();
    [[nodiscard]] inline uint32_t getOutput() const { return output; }
    [[nodiscard]] inline bool isBackgroundEnabled() const { return enableBackground; }
    [[nodiscard]] inline uint32_t getLayer() const { return layer; }
    [[nodiscard]] inline float getFallingPortalProbability() const { return fallingPortalProbability; }
private:
    static constexpr const char *CONFIG_FILE = "shanghai.json";
    static ShanghaiConfiguration *instance;

    ShanghaiConfiguration();
    ~ShanghaiConfiguration();
    void serialize(Json::Value &root) const;
    void deserialize(const Json::Value &root);

    uint32_t output = UINT32_MAX;
    bool enableBackground = false;
    uint32_t layer = 1;
    float fallingPortalProbability = 0.0f;
};


#endif //SHANGHAI_SHANGHAICONFIGURATION_H
