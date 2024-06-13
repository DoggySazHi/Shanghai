#include <stdexcept>
#include "Background.h"

Background::Background() {
    shader1 = new Shader("shader/background.vert", "shader/background1.frag", "shader/backgroundcommon.frag");
    shader2 = new Shader("shader/background.vert", "shader/background2.frag", "shader/backgroundcommon.frag");
    shader3 = new Shader("shader/background.vert", "shader/background3.frag", "shader/backgroundcommon.frag");

    if (!shader1->isCompiled() || !shader2->isCompiled() || !shader3->isCompiled()) {
        throw std::runtime_error("Failed to compile shaders");
    }

    startTime = (float) getTime();
}

Background::~Background() {
    delete shader1;
}

void Background::draw(EGLState* state) {
    shader1->use();

    // Rendered as a full quad
    static const GLfloat vertices[] = {
            -1, 1,
            1, 1,
            -1, -1,
            1, -1,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    shader1->setUniform("iResolution", (float) displayWidth, (float) displayHeight);
    shader1->setUniform("iTime", 0, 0, 0, startTime - ((float) getTime() / 1000.0f));
    shader1->setUniform("iDate", (float) getTime() / 1000.0f);
    shader1->setUniform("iFrame", frame++);

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
