#ifndef SHANGHAI_SHANGHAIANIMATIONMACHINE_H
#define SHANGHAI_SHANGHAIANIMATIONMACHINE_H


#include <vector>
#include <cstdint>

struct ShanghaiAnimationFrame {
    int textureIndex;
    int duration;
    float translationX;
    float translationY;

    ShanghaiAnimationFrame(int textureIndex, int duration, float translationX = 0, float translationY = 0) {
        this->textureIndex = textureIndex;
        this->duration = duration;
        this->translationX = translationX;
        this->translationY = translationY;
    }
};

class ShanghaiAnimationMachine {
public:
    explicit ShanghaiAnimationMachine(const std::vector<ShanghaiAnimationFrame>& frames);
    ShanghaiAnimationFrame& getFrame(uint64_t time);
    void setAnimationFactor(uint32_t factor);
    void setReferenceTime(uint64_t time);
private:
    uint32_t durationFactor = 33; // Factor to durationFactor the animation speed (Milliseconds per frame)
    uint64_t referenceTime = 0;

    std::vector<ShanghaiAnimationFrame> frames;
    uint32_t totalDuration = 0;
};


#endif //SHANGHAI_SHANGHAIANIMATIONMACHINE_H
