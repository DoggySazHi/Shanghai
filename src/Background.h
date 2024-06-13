#ifndef SHANGHAI_BACKGROUND_H
#define SHANGHAI_BACKGROUND_H


#include <chrono>
#include "Shader.h"
#include "state.h"

class Background {
public:
    Background();
    ~Background();
    void draw(EGLState* state);
    void setScreenGeometry(uint32_t width, uint32_t height);
    static uint64_t getTime();
private:
    int frame = 0;
    float startTime = 0;

    Shader* shader1;
    Shader* shader2;
    Shader* shader3;

    uint32_t displayWidth = 0, displayHeight = 0;
};


#endif //SHANGHAI_BACKGROUND_H
