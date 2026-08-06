#include "NinePatch.h"

#include <stdexcept>
#include "stb_image.h"

Shader* NinePatch::shader = nullptr;

NinePatch::NinePatch(const std::string& filePath, const int x, const int y, const int dx, const int dy) : x(x), y(y), dx(dx), dy(dy) {
    if (shader == nullptr) {
        shader = new Shader("shader/ninepatch.vert", "shader/ninepatch.frag");
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &this->textureWidth, &this->textureHeight, &channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textureWidth, this->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        throw std::runtime_error("Failed to load texture: " + filePath);
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    shader->use();
    shader->setUniform("ninePatchGeometry", (float) x, (float) y, (float) dx, (float) dy);
}

NinePatch::~NinePatch() {
    glDeleteTextures(1, &texture);
}

void NinePatch::render(const EGLState* state, int x, int y, int width, int height) const {
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader->setUniform("patchTexture", 0);
    shader->setUniform("screenGeometry", (float) state->width, (float) state->height);
    shader->setUniform("position", (float) x, (float) y, (float) width, (float) height);
    shader->setUniform("patchShape", (float) this->textureWidth, (float) this->textureHeight);
    shader->setUniform("patchGeometry", (float) this->x, (float) this->y, (float) this->dx, (float) this->dy);

    // Render the 9-patch as a quad
    GLfloat vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, (float) height, 0.0f, 1.0f,
        (float) width, 0.0f, 1.0f, 0.0f,
        (float) width, (float) height, 1.0f, 1.0f,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D, 0);
}
