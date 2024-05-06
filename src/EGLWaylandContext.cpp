#include <iostream>
#include <cstring>
#include "EGLWaylandContext.h"

const EGLint configAttributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_ALPHA_SIZE, 1,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE,
};

const EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE,
};

bool EGLWaylandContext::init(wl_display* waylandDisplay) {
    const char *availableEGLExtensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);

    if (availableEGLExtensions == nullptr) {
        if (eglGetError() == EGL_BAD_DISPLAY) {
            std::cerr << "EGL_EXT_client_extensions not supported\n";
        } else {
            std::cerr << "Failed to query EGL client extensions\n";
        }
        return false;
    }

    if (!strstr(availableEGLExtensions, "EGL_EXT_platform_base")) {
        std::cerr << "EGL_EXT_platform_base not supported\n";
        return false;
    }

    if (!strstr(availableEGLExtensions, "EGL_EXT_platform_wayland")) {
        std::cerr << "EGL_EXT_platform_wayland not supported\n";
        return false;
    }

    getPlatformDisplayExt = (PFNEGLGETPLATFORMDISPLAYEXTPROC) eglGetProcAddress("eglGetPlatformDisplayEXT");
    if (getPlatformDisplayExt == nullptr) {
        std::cerr << "Failed to get eglGetPlatformDisplayEXT\n";
        return false;
    }

    createPlatformWindowSurfaceExt = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC) eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");
    if (createPlatformWindowSurfaceExt == nullptr) {
        std::cerr << "Failed to get eglCreatePlatformWindowSurfaceEXT\n";
        return false;
    }

    display = getPlatformDisplayExt(EGL_PLATFORM_WAYLAND_EXT, waylandDisplay, nullptr);
    if (display == EGL_NO_DISPLAY) {
        std::cerr << "Failed to create EGL display\n";
        finish();
        return false;
    }

    if (eglInitialize(display, nullptr, nullptr) == EGL_FALSE) {
        std::cerr << "Failed to initialize EGL\n";
        finish();
        return false;
    }

    EGLint matched = 0;
    if (!eglChooseConfig(display, configAttributes, &config, 1, &matched)) {\
        std::cerr << "eglChooseConfig failed\n";
        finish();
        return false;
    }

    if (matched == 0) {
        std::cerr << "Failed to match an EGL config\n";
        finish();
        return false;
    }

    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
    if (context == EGL_NO_CONTEXT) {
        std::cerr << "Failed to create EGL context\n";
        finish();
        return false;
    }

    return true;
}

void EGLWaylandContext::finish() {
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    // Terminate the display and context, if applicable (used also during errors)
    if (display != EGL_NO_DISPLAY) {
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }

        eglTerminate(display);
    }

    eglReleaseThread();
}

EGLWaylandContext::EGLWaylandContext(wl_display *waylandDisplay) {
    if (!init(waylandDisplay)) {
        std::cerr << "Failed to initialize EGLWaylandContext\n";
    }
}

EGLWaylandContext::~EGLWaylandContext() {
    finish();
}

EGLSurface EGLWaylandContext::createSurface(wl_egl_window* window) {
    return createPlatformWindowSurfaceExt(display, config, window, nullptr);
}

void EGLWaylandContext::destroySurface(wl_egl_window *window, EGLSurface eglSurface) {
    eglDestroySurface(display, eglSurface);
}

void EGLWaylandContext::makeCurrent(EGLSurface eglSurface) {
    eglMakeCurrent(display, eglSurface, eglSurface, context);
}

void EGLWaylandContext::swapBuffers(EGLSurface eglSurface) {
    eglSwapBuffers(display, eglSurface);
}
