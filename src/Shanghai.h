#ifndef SHANGHAI_SHANGHAI_H
#define SHANGHAI_SHANGHAI_H


#include "Shader.h"

#define SHANGHAI_TEXTURE_COUNT 46

class Shanghai {
public:
    Shanghai();
    ~Shanghai();
    void draw();
    void nextTexture();
    void setScreenGeometry(uint32_t width, uint32_t height);
private:
    Shader* shader;
    GLuint textures[SHANGHAI_TEXTURE_COUNT] = {0};
    int textureIndex = 0;
    uint32_t displayWidth = 0, displayHeight = 0;
};


#endif //SHANGHAI_SHANGHAI_H
