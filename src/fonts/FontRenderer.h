#ifndef SHANGHAI_FONTRENDERER_H
#define SHANGHAI_FONTRENDERER_H

#include "BDF.h"
#include "../Shader.h"
#include "glad/glad.h"
#include "../state.h"

struct FontTexture {
    int offsetX;
    int offsetY;
    int width;
    int height;
};

class FontRenderer {
public:
    FontRenderer(const std::string& fontFile);
    ~FontRenderer();
    void testRenderAtlas(const EGLState* state) const;
    void renderCharacter(const EGLState *state, char character, int x, int y, int r, int g, int b, int a) const;
    void renderString(const EGLState *state, const std::string &str, int x, int y, int r, int g, int b, int a) const;
private:
    static Shader* shader; // Shader to use for rendering
    BDF* font; // Font to render
    std::unordered_map<char, FontTexture> positionMap; // Map of character to atlas position
    GLuint atlasTexture = 0; // Texture atlas for the font
};



#endif //SHANGHAI_FONTRENDERER_H
