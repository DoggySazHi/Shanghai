#include <stdexcept>
#include "Shanghai.h"
#include "stb_image.h"

Shanghai::Shanghai() {
    shader = new Shader("shader/shanghai.vert", "shader/shanghai.frag");

    if (!shader->isCompiled()) {
        throw std::runtime_error("Failed to compile shader");
    }

    glGenTextures(SHANGHAI_TEXTURE_COUNT, textures);
    for (int i = 0; i < SHANGHAI_TEXTURE_COUNT; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int imgWidth, imgHeight, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        std::string path = "img/shime" + std::to_string(i + 1) + ".png";
        unsigned char *data = stbi_load(path.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            throw std::runtime_error("Failed to load texture " + path);
        }
        stbi_image_free(data);
    }

    stateMachine = new ShanghaiStateMachine();
}

Shanghai::~Shanghai() {
    delete shader;
    glDeleteTextures(SHANGHAI_TEXTURE_COUNT, textures);
    delete stateMachine;
}

void Shanghai::draw(EGLState* state) {
    stateMachine->frame(this, state);

    shader->use();

    // Packed as [x, y, u, v]
    // Renders a square
    static const GLfloat vertices[] = {
            0, 0, 0.0, 0.0,
            128, 0, 1.0, 0.0,
            128, 128, 1.0, 1.0,
            128, 128, 1.0, 1.0,
            0, 128, 0.0, 1.0,
            0, 0, 0.0, 0.0,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("shanghaiTexture", 0);
    shader->setUniform("screenGeometry", (float) displayWidth, (float) displayHeight);
    shader->setUniform("shanghaiPosition", positionX, positionY);
    shader->setUniform("shanghaiHFlip", flip);
    glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Shanghai::setScreenGeometry(uint32_t width, uint32_t height) {
    displayWidth = width;
    displayHeight = height;
}

void Shanghai::setTexture(int index) {
    if (index < 0 || index >= SHANGHAI_TEXTURE_COUNT) {
        throw std::runtime_error("Invalid texture index " + std::to_string(index) + " out of bounds 0-" + std::to_string(SHANGHAI_TEXTURE_COUNT - 1) + ".");
    }

    textureIndex = index;
}

// Gets the relative time in milliseconds
uint64_t Shanghai::getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
