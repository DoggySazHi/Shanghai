#define GLFW_INCLUDE_NONE

#include "../platform/Platform.h"

namespace {
    HWND windowHandle;

    // Shove the window back to the bottom of the Z order where it belongs.
    void sinkToBottom() {
        SetWindowPos(windowHandle, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    void focusHandler([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] int focused) {
        // Prevent clicks from changing z-order
        sinkToBottom();
    }
}

namespace platform {
    void applyWindowHints() {
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    }

    void afterWindowCreated(GLFWwindow* window,
                            [[maybe_unused]] int width,
                            [[maybe_unused]] int height) {
        windowHandle = glfwGetWin32Window(window);
        // Hide the window from the taskbar
        ShowWindow(windowHandle, SW_HIDE);
        SetWindowLong(windowHandle, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
        ShowWindow(windowHandle, SW_SHOW);
    }

    void beforeMainLoop([[maybe_unused]] GLFWwindow* window,
                        [[maybe_unused]] int width,
                        [[maybe_unused]] int height) {
        // Nothing to do
    }

    void installCallbacks(GLFWwindow* window) {
        glfwSetWindowFocusCallback(window, focusHandler);
    }

    void afterSwap([[maybe_unused]] GLFWwindow* window) {
        sinkToBottom();
    }
}
