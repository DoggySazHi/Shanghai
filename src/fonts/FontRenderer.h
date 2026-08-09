#ifndef SHANGHAI_FONTRENDERER_H
#define SHANGHAI_FONTRENDERER_H

#include "BDF.h"
#include "../Shader.h"
#include "../gl.h"
#include "../gl/Quad.h"
#include "../state.h"

struct FontTexture {
    int offsetX;
    int offsetY;
    int width;
    int height;
};

struct TextSize {
    int width;
    int height;
};

class FontRenderer {
public:
    FontRenderer(const std::string& fontFile);
    ~FontRenderer();
    void testRenderAtlas(const EGLState* state) const;
    void renderCharacter(const EGLState *state, char character, int x, int y, int r, int g, int b, int a) const;
    void renderString(const EGLState *state, std::string_view str, int x, int y, int r, int g, int b, int a) const;
    [[nodiscard]] TextSize measureString(std::string_view str) const;
private:
    static Shader* shader; // Shader to use for rendering
    Quad quad{Quad::Layout::POSITION_AND_UV}; // Geometry each glyph is drawn onto
    BDF* font; // Font to render
    std::unordered_map<char, FontTexture> positionMap; // Map of character to atlas position
    GLuint atlasTexture = 0; // Texture atlas for the font
};



#endif //SHANGHAI_FONTRENDERER_H
