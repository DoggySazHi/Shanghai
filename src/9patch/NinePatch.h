#ifndef SHANGHAI_NINEPATCH_H
#define SHANGHAI_NINEPATCH_H

#include <string>
#include "../Shader.h"
#include "glad/glad.h"
#include "../state.h"

class NinePatch {
public:
    // x/y are the coordinates of the top-left corner of the center of the 9patch image
    // dx/dy are the width/height of the center of the 9patch image
    // other quadrants are calculated based on the center
    NinePatch(const std::string& filePath, int x, int y, int dx, int dy);
    ~NinePatch();

    void render(const EGLState *state, int x, int y, int width, int height) const;
private:
    static Shader* shader; // Shader to use for rendering
    GLuint texture = 0; // Texture for the 9patch image
    int x, y, dx, dy; // Coordinates and dimensions of the 9patch
    int textureWidth = 0, textureHeight = 0;
};



#endif //SHANGHAI_NINEPATCH_H
