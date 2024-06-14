#include <stdexcept>
#include <iostream>
#include "Background.h"
#include "Random.h"
#include "glad/glad.h"

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
    delete shader2;
    delete shader3;
}

void Background::draw(EGLState* state) {
    if (!texturesInitialized) {
        setScreenGeometry(state->width, state->height);
    }

    glDisable(GL_BLEND);

    shader1->use();
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
    drawInternal(shader1);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextures[0], 0);

    shader2->use();
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTextures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderedTextures[bufferFlip ? 1 : 1]);
    shader2->setUniform("iChannel0", 0);
    shader2->setUniform("iChannel1", bufferFlip ? 1 : 1);
    drawInternal(shader2);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextures[bufferFlip ? 1 : 1], 0);

    shader3->use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTextures[bufferFlip ? 1 : 1]);
    shader3->setUniform("iChannel0", 1);
    drawInternal(shader3);

//    bufferFlip = !bufferFlip;

    glEnable(GL_BLEND);
}

void Background::drawInternal(Shader *shader) {
//    shader->use();

    // Rendered as a full quad
    static const GLfloat vertices[] = {
            -1, 1,
            1, 1,
            -1, -1,
            1, -1,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    shader->setUniform("iResolution", (float) displayWidth, (float) displayHeight);
    shader->setUniform("iTime", 0, 0, 0, startTime - ((float) getTime() / 1000.0f));
    shader->setUniform("iDate", (float) getTime() / 1000.0f);
    shader->setUniform("iFrame", frame++);
    shader->setUniform("iRng", Random::rand());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
}

void Background::setScreenGeometry(uint32_t width, uint32_t height) {
    displayWidth = width;
    displayHeight = height;

    if (texturesInitialized || displayWidth == 0 || displayHeight == 0) return;

    glGenTextures(1, &renderedTextures[0]);
    glBindTexture(GL_TEXTURE_2D, renderedTextures[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (int) displayWidth, (int) displayHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &frameBuffers[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextures[0], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer A " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
    }

    glGenTextures(1, &renderedTextures[1]);
    glBindTexture(GL_TEXTURE_2D, renderedTextures[1]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (int) displayWidth, (int) displayHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &frameBuffers[1]);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[1]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTextures[1], 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer B " + std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
    }

    glGenTextures(1, &renderedTextures[2]);
    glBindTexture(GL_TEXTURE_2D, renderedTextures[2]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (int) displayWidth, (int) displayHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    texturesInitialized = true;
}

/**
 * Get the current time in milliseconds. This uses a monotonic clock.
 * @return The current time in milliseconds
 */
uint64_t Background::getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}
