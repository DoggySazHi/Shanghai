#define GLFW_INCLUDE_NONE

#include "../platform/Platform.h"

namespace platform {
    void applyWindowHints() {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

        // glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    }

    void afterWindowCreated([[maybe_unused]] GLFWwindow* window,
                            [[maybe_unused]] int width,
                            [[maybe_unused]] int height) {
        // Nothing to do
    }

    void beforeMainLoop(GLFWwindow* window, const int width, const int height) {
        // Force realignment of window after window creation
        XSetWindowBorderWidth(glfwGetX11Display(), glfwGetX11Window(window), 0);
        XMoveResizeWindow(glfwGetX11Display(), glfwGetX11Window(window), 0, 0, width, height);
    }

    void installCallbacks([[maybe_unused]] GLFWwindow* window) {
        // Nothing to do
    }

    void afterSwap([[maybe_unused]] GLFWwindow* window) {
        // Nothing to do
    }
}
