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
    void drawInternal(Shader* shader);

    int frame = 0;
    float startTime = 0;

    bool texturesInitialized = false;
    GLuint frameBuffers[2] = {0};
    GLuint depthBuffers[2] = {0};
    GLuint renderedTextures[3] = {0};
    bool bufferFlip = false;

    Shader* shader1;
    Shader* shader2;
    Shader* shader3;

    uint32_t displayWidth = 0, displayHeight = 0;
};


#endif //SHANGHAI_BACKGROUND_H
