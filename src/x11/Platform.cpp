#define GLFW_INCLUDE_NONE

#include "../platform/Platform.h"

#include <X11/Xutil.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

namespace {
    _XDisplay* xDisplay = nullptr;
    Window xWindow = 0;

    void grabHandles(GLFWwindow* window) {
        if (xDisplay == nullptr) {
            xDisplay = glfwGetX11Display();
            xWindow = glfwGetX11Window(window);
        }
    }
}

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

    void beforeMainLoop(GLFWwindow* window,
                        [[maybe_unused]] int x,
                        [[maybe_unused]] int y,
                        const int width,
                        const int height) {
        // Force realignment of window after window creation
        XSetWindowBorderWidth(glfwGetX11Display(), glfwGetX11Window(window), 0);
        XMoveResizeWindow(glfwGetX11Display(), glfwGetX11Window(window), x, y, width, height);
    }

    void installCallbacks([[maybe_unused]] GLFWwindow* window) {
        // Nothing to do
    }

    void afterSwap([[maybe_unused]] GLFWwindow* window) {
        // Nothing to do
    }

    void updatePointer(GLFWwindow* window, EGLState* state, const std::vector<InputRect>& interactive) {
        // Hard-coded 20 frame delay in terms of recalculating the X11 clickable region
        // Running every frame causes XShapeCombineRegion to lag
        static uint8_t frameCounter = 0;
        if (frameCounter > 20) {
            frameCounter = 0;
        } else {
            frameCounter++;
            return;
        }

        grabHandles(window);

        // Update X11 cursor position
        Window rootReturn, childReturn;
        int rootXReturn, rootYReturn, winXReturn, winYReturn;
        unsigned int maskReturn;

        bool isInWindow = XQueryPointer(xDisplay, xWindow, &rootReturn, &childReturn, &rootXReturn, &rootYReturn, &winXReturn, &winYReturn, &maskReturn);

        if (!isInWindow) {
            state->curX = rootXReturn;
            state->curY = rootYReturn;
        } else {
            state->curX = winXReturn;
            state->curY = winYReturn;
        }

        // Handle clickable regions
        auto region = XCreateRegion();

        for (const auto& rect : interactive) {
            XRectangle xRect;
            xRect.x = (short) rect.x;
            xRect.y = (short) rect.y;
            xRect.width = (unsigned short) rect.width;
            xRect.height = (unsigned short) rect.height;
            XUnionRectWithRegion(&xRect, region, region);
        }

        XShapeCombineRegion(xDisplay, xWindow, ShapeInput, 0, 0, region, ShapeSet);
        XDestroyRegion(region);
    }

    void warpCursor(GLFWwindow* window, const int x, const int y) {
        grabHandles(window);

        XWarpPointer(xDisplay, None, xWindow, 0, 0, 0, 0, x, y);
    }
}
