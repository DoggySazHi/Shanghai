#ifndef SHANGHAI_PLATFORM_H
#define SHANGHAI_PLATFORM_H

#include <vector>

#include "../gl.h"
#include "../state.h"

void resizeHandler(GLFWwindow* window, int width, int height);

// GLFW OS-specific changes
namespace platform {
    // Interactive regions, i.e., where Shanghais are
    struct InputRect {
        int x, y, width, height;
    };

    void applyWindowHints();
    void afterWindowCreated(GLFWwindow* window, int width, int height);
    void beforeMainLoop(GLFWwindow* window, int x, int y, int width, int height);
    void installCallbacks(GLFWwindow* window);
    void afterSwap(GLFWwindow* window);
    void updatePointer(GLFWwindow* window, EGLState* state, const std::vector<InputRect>& interactive);
    void warpCursor(GLFWwindow* window, int x, int y);
}

#endif //SHANGHAI_PLATFORM_H
