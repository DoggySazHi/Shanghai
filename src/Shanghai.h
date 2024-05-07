#ifndef SHANGHAI_SHANGHAI_H
#define SHANGHAI_SHANGHAI_H


#include <chrono>
#include "Shader.h"
#include "state.h"
#include "ShanghaiStateMachine.h"

#define SHANGHAI_TEXTURE_COUNT 46

// Forward declaration
class ShanghaiStateMachine;

class Shanghai {
public:
    Shanghai();
    ~Shanghai();
    void draw(EGLState* state);
    void nextTexture();
    void setTexture(int index);
    void setScreenGeometry(uint32_t width, uint32_t height);
    void setPos(float x, float y);
    void setFlip(bool isFlipped);
    static uint64_t getTime();
private:
    Shader* shader;
    GLuint textures[SHANGHAI_TEXTURE_COUNT] = {0};
    int textureIndex = 0;
    uint32_t displayWidth = 0, displayHeight = 0;
    float positionX = 0, positionY = 0;
    bool flip = false;

    ShanghaiStateMachine* stateMachine;
};


#endif //SHANGHAI_SHANGHAI_H
