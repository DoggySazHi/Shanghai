#define GLFW_INCLUDE_NONE

#include "../platform/Platform.h"

#import <Cocoa/Cocoa.h>

#include <CoreGraphics/CoreGraphics.h>

namespace {
    // macOS only allows the entire app to be interactive or not, so we cheat by only making it interactive when hovered
    bool interactive = false;
    bool grabbed = false;
    bool wasPressed = false;

    NSWindow* cocoaWindow(GLFWwindow* window) {
        return (NSWindow*) glfwGetCocoaWindow(window);
    }

    double backingScale(GLFWwindow* window) {
        // Note that these already exist in GlfwMain as cursorScaleX/cursorScaleY, but macOS requires these specifically
        int windowWidth, windowHeight, framebufferWidth, framebufferHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

        return windowWidth > 0 ? (double) framebufferWidth / windowWidth : 1.0;
    }

    // Core Graphics origin (which is not equal to our monitor origin from GLFW)
    CGFloat primaryScreenTop() {
        NSScreen* primary = [[NSScreen screens] firstObject];
        return primary == nil ? 0 : NSMaxY([primary frame]);
    }

    // Core Graphics monitor object derived from GLFW window position
    NSScreen* screenAt(const int x, const int y) {
        const NSPoint point = NSMakePoint(x + 0.5, primaryScreenTop() - y - 0.5);

        for (NSScreen* screen in [NSScreen screens]) {
            if (NSPointInRect(point, [screen frame])) {
                return screen;
            }
        }

        return nil;
    }

    void setInteractive(GLFWwindow* window, const bool value) {
        if (interactive == value) {
            return;
        }

        interactive = value;
        glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, value ? GLFW_FALSE : GLFW_TRUE);
    }
}

namespace platform {
    void applyWindowHints() {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, GLFW_TRUE);
    }

    void afterWindowCreated(GLFWwindow* window,
                            [[maybe_unused]] int width,
                            [[maybe_unused]] int height) {
        NSWindow* nsWindow = cocoaWindow(window);

        if (nsWindow == nil) {
            return;
        }

        // Put Shanghai above everything else
        [nsWindow setLevel:NSScreenSaverWindowLevel];
        // Follow between spaces, stay out of Mission Control and command-tab
        [nsWindow setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces |
                                        NSWindowCollectionBehaviorStationary |
                                        NSWindowCollectionBehaviorIgnoresCycle |
                                        NSWindowCollectionBehaviorFullScreenNone];
        [nsWindow setHasShadow:NO];
    }

    void beforeMainLoop(GLFWwindow* window,
                        const int x,
                        const int y,
                        [[maybe_unused]] int width,
                        [[maybe_unused]] int height) {
        NSWindow* nsWindow = cocoaWindow(window);

        if (nsWindow == nil) {
            return;
        }

        // XMoveResizeWindow but with extra steps to cover the full monitor
        NSScreen* target = screenAt(x, y);

        if (target == nil) {
            target = [nsWindow screen] ?: [NSScreen mainScreen];
        }

        if (target != nil) {
            [nsWindow setFrame:[target frame] display:YES];
        }
    }

    void installCallbacks(GLFWwindow* window) {
        // glfwSetFramebufferSizeCallback is not enough on macOS...
        glfwSetWindowSizeCallback(window, resizeHandler);
    }

    void afterSwap([[maybe_unused]] GLFWwindow* window) {
        // Nothing to do
    }

    void updatePointer(GLFWwindow* window, EGLState* state, const std::vector<InputRect>& interactiveRects) {
        NSWindow* nsWindow = cocoaWindow(window);

        if (nsWindow == nil) {
            return;
        }

        // Need to get the mouse position directly since GLFW will not report when non-interactive
        const NSRect frame = [nsWindow frame];
        const NSPoint mouse = [NSEvent mouseLocation];
        const double scale = backingScale(window);

        state->curX = (int) ((mouse.x - NSMinX(frame)) * scale);
        state->curY = (int) ((NSMaxY(frame) - mouse.y) * scale);

        bool onShanghai = false;
        for (const auto& rect : interactiveRects) {
            if (state->curX >= rect.x && state->curX < rect.x + rect.width &&
                state->curY >= rect.y && state->curY < rect.y + rect.height) {
                onShanghai = true;
                break;
            }
        }

        const bool pressed = [NSEvent pressedMouseButtons] != 0;

        if (!pressed) {
            grabbed = false;
        } else if (!wasPressed && onShanghai && interactive) {
            grabbed = true;
        }

        wasPressed = pressed;
        setInteractive(window, onShanghai || grabbed);

        if (grabbed) {
            state->buttons = 1;
        } else if (!pressed) {
            state->buttons = 0;
        }
    }

    void warpCursor(GLFWwindow* window, const int x, const int y) {
        NSWindow* nsWindow = cocoaWindow(window);

        if (nsWindow == nil) {
            return;
        }

        const NSRect frame = [nsWindow frame];
        const double scale = backingScale(window);

        // Window-relative framebuffer pixels to CoreGraphics' flipped global points
        const CGPoint target = CGPointMake(NSMinX(frame) + x / scale,
                                           primaryScreenTop() - NSMaxY(frame) + y / scale);

        CGWarpMouseCursorPosition(target);
        // Warping otherwise mutes mouse movement for a quarter of a second
        CGAssociateMouseAndMouseCursorPosition(true);
    }
}
