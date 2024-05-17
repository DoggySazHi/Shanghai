#include <stdexcept>
#include "Background.h"

Background::Background() {
    shader = new Shader("shader/background.vert", "shader/background.frag");

    if (!shader->isCompiled()) {
        throw std::runtime_error("Failed to compile shader");
    }
}

Background::~Background() {
    delete shader;
}

void Background::draw(EGLState* state) {
    shader->use();

    // Rendered as a full quad
    static const GLfloat vertices[] = {
            -1, 1,
            1, 1,
            -1, -1,
            1, -1,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    shader->setUniform("time", (float) getTime() / 1000.0f);
    shader->setUniform("resolution", (float) displayWidth, (float) displayHeight);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);

    glDisableVertexAttribArray(0);
}

void Background::setScreenGeometry(uint32_t width, uint32_t height) {
    displayWidth = width;
    displayHeight = height;
}

/**
 * Get the current time in milliseconds. This uses a monotonic clock.
 * @return The current time in milliseconds
 */
uint64_t Background::getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
