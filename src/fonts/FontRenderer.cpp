#include "FontRenderer.h"

#include <ranges>

Shader* FontRenderer::shader = nullptr;

void copyRectangle(unsigned char* src, int srcWidth, int srcHeight,
                   unsigned char* dst, int dstWidth, int dstHeight,
                   int destX, int destY) {
    for (int x = 0; x < srcWidth; ++x) {
        for (int y = 0; y < srcHeight; ++y) {
            int srcIndex = srcWidth * y + x;
            int dstIndex = dstWidth * (destY + y) + (destX + x);
            if (destX + x < dstWidth && destY + y < dstHeight) {
                dst[dstIndex] = src[srcIndex]; // Copy pixel from source to destination
            }
        }
    }
}

#define ATLAS_SIZE 256

FontRenderer::FontRenderer(const std::string& fontFile) {
    if (shader == nullptr) {
        shader = new Shader("shader/font.vert", "shader/font.frag");
    }

    font = new BDF(fontFile);

    auto* chars = font->getChars();
    const int charCount = font->getCharCount();

    int maxHeight = 0;
    int maxWidth = 0;

    for (int i = 0; i < charCount; ++i) {
        BDFChar *character = &chars[i];
        maxHeight = std::max(maxHeight, character->height);
        maxWidth = std::max(maxWidth, character->width);
    }

    auto* atlasData = new unsigned char[ATLAS_SIZE * ATLAS_SIZE]; // Assuming a maximum atlas size of 1024x1024
    for (int i = 0; i < ATLAS_SIZE * ATLAS_SIZE; ++i) {
        atlasData[i] = 0x00; // Initialize the atlas with black pixels
    }

    std::vector<unsigned char> characterData;
    int currentX = 0;
    int currentY = 0;

    for (int i = 0; i < charCount; ++i) {
        BDFChar *character = &chars[i];

        characterData.clear();
        for (auto bit : character->bitmap) {
            if (bit) {
                characterData.push_back(0xFF); // white pixel
            } else {
                characterData.push_back(0x00); // black pixel
            }
        }

        copyRectangle(characterData.data(), character->width, character->height,
                     atlasData, ATLAS_SIZE, ATLAS_SIZE,
                     currentX, currentY);

        currentX += maxWidth;
        if (currentX + maxWidth > ATLAS_SIZE) {
            currentX = 0;
            currentY += maxHeight; // Move to the next row
        }

        if (currentY + maxHeight > ATLAS_SIZE) {
            throw std::runtime_error("Font atlas size exceeded. Increase ATLAS_SIZE or reduce character sizes.");
        }

        positionMap[character->character] = {
            currentX,
            currentY,
            character->width,
            character->height
        };
    }

    // Vertically flip the atlas data
    for (int y = 0; y < ATLAS_SIZE / 2; ++y) {
        for (int x = 0; x < ATLAS_SIZE; ++x) {
            std::swap(atlasData[y * ATLAS_SIZE + x], atlasData[(ATLAS_SIZE - 1 - y) * ATLAS_SIZE + x]);
        }
    }

    // Vertically flip the character positions
    for (auto& [character, texture] : positionMap) {
        texture.offsetY = ATLAS_SIZE - texture.offsetY - texture.height; // Adjust Y offset
    }

    glGenTextures(1, &atlasTexture);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ATLAS_SIZE, ATLAS_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

FontRenderer::~FontRenderer() {
    glDeleteTextures(1, &atlasTexture);
    delete font;
}

void FontRenderer::testRenderAtlas(const EGLState* state) const {
    shader->use();

    // Packed as [x, y, u, v]
    // Renders a square
    static constexpr GLfloat vertices[] = {
        0, 0, 0.0, 0.0,
        0, ATLAS_SIZE, 0.0, ATLAS_SIZE,
        ATLAS_SIZE, 0, ATLAS_SIZE, 0.0,
        ATLAS_SIZE, ATLAS_SIZE, ATLAS_SIZE, ATLAS_SIZE,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("fontTexture", 0);
    shader->setUniform("screenGeometry", (float) state->width, (float) state->height);
    shader->setUniform("fontShape", (float) ATLAS_SIZE, (float) ATLAS_SIZE);
    shader->setUniform("position", 0, 0);
    shader->setUniform("color", 255.0f, 255.0f, 255.0f, 255.0f);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void FontRenderer::renderCharacter(const EGLState* state, char character, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255) const {
    const auto it = positionMap.find(character);
    if (it == positionMap.end()) {
        return; // Character not found in the atlas
    }

    const FontTexture& texture = it->second;
    // ReSharper disable once CppDFANullDereference
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    shader->setUniform("fontTexture", 0);
    shader->setUniform("screenGeometry", (float) state->width, (float) state->height);
    shader->setUniform("fontShape", (float) ATLAS_SIZE, (float) ATLAS_SIZE);
    shader->setUniform("position", x, y);
    shader->setUniform("color", (float) r, (float) g, (float) b, (float) a);

    // Render the character as a quad
    GLfloat vertices[] = {
        0.0f, 0.0f, (float) (texture.offsetX - texture.width), (float) texture.offsetY,
        0.0f, (float) texture.height, (float) (texture.offsetX - texture.width), (float) (texture.offsetY + texture.height),
        (float) texture.width, 0.0f, (float) texture.offsetX, (float) texture.offsetY,
        (float) texture.width, (float) texture.height, (float) texture.offsetX, (float) (texture.offsetY + texture.height),
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void FontRenderer::renderString(const EGLState* state, const std::string& str, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255) const {
    int curX = x;
    int curY = y;

    for (char character : str) {
        auto it = positionMap.find(character);

        if (it == positionMap.end()) {
            // If the character is not found, skip it
            continue;
        }

        if (character == '\n') {
            curX = x; // Reset X position for new line
            curY += it->second.height; // Move down by line height
            continue;
        }

        renderCharacter(state, character, curX, curY, r, g, b, a);
        curX += it->second.width; // Move to the next character position
    }
}