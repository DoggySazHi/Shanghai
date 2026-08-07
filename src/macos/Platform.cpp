#define GLFW_INCLUDE_NONE

#include "../platform/Platform.h"

namespace platform {
    void applyWindowHints() {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    }

    void afterWindowCreated([[maybe_unused]] GLFWwindow* window,
                            [[maybe_unused]] int width,
                            [[maybe_unused]] int height) {
        // Nothing to do
    }

    void beforeMainLoop([[maybe_unused]] GLFWwindow* window,
                        [[maybe_unused]] int x,
                        [[maybe_unused]] int y,
                        [[maybe_unused]] int width,
                        [[maybe_unused]] int height) {
        // Nothing to do
    }

    void installCallbacks(GLFWwindow* window) {
        // glfwSetFramebufferSizeCallback is not enough on macOS...
        glfwSetWindowSizeCallback(window, resizeHandler);
    }

    void afterSwap([[maybe_unused]] GLFWwindow* window) {
        // Nothing to do
    }
}
