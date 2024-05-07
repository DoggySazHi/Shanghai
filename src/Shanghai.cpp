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
}

Shanghai::~Shanghai() {
    delete shader;
}

void Shanghai::draw() {
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
    glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Shanghai::nextTexture() {
    textureIndex = (textureIndex + 1) % SHANGHAI_TEXTURE_COUNT;
}

void Shanghai::setScreenGeometry(uint32_t width, uint32_t height) {
    displayWidth = width;
    displayHeight = height;
}
