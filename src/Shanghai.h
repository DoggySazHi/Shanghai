#ifndef SHANGHAI_SHANGHAI_H
#define SHANGHAI_SHANGHAI_H


#include <chrono>
#include "Shader.h"
#include "state.h"
#include "ShanghaiStateMachine.h"

#define SHANGHAI_TEXTURE_COUNT 46
#define SHANGHAI_TEXTURE_WIDTH 128

// Forward declaration
class ShanghaiStateMachine;

class Shanghai {
public:
    Shanghai();
    ~Shanghai();
    void draw(EGLState* state);
    void setTexture(int index);
    void setScreenGeometry(uint32_t width, uint32_t height);
    static uint64_t getTime();

    float positionX = 0, positionY = 0;
    bool flip = true;
private:
    void updateCursor(EGLState* state) const;

    Shader* shader;
    GLuint textures[SHANGHAI_TEXTURE_COUNT] = {0};
    int textureIndex = 0;
    uint32_t displayWidth = 0, displayHeight = 0;

    ShanghaiStateMachine* stateMachine;
};


#endif //SHANGHAI_SHANGHAI_H
