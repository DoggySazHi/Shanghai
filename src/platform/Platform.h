#ifndef SHANGHAI_PLATFORM_H
#define SHANGHAI_PLATFORM_H

#include "../gl.h"

void resizeHandler(GLFWwindow* window, int width, int height);

// GLFW OS-specific changes
namespace platform {
    void applyWindowHints();
    void afterWindowCreated(GLFWwindow* window, int width, int height);
    void beforeMainLoop(GLFWwindow* window, int width, int height);
    void installCallbacks(GLFWwindow* window);
    void afterSwap(GLFWwindow* window);
}

#endif //SHANGHAI_PLATFORM_H
