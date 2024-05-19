#ifndef SHANGHAI_EGLWAYLANDCONTEXT_H
#define SHANGHAI_EGLWAYLANDCONTEXT_H

#include <wayland-client.h>
#include <wayland-egl-backend.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>


class EGLWaylandContext {
public:
    explicit EGLWaylandContext(wl_display* waylandDisplay);
    ~EGLWaylandContext();
    EGLSurface createSurface(wl_egl_window* window);
    void destroySurface(EGLSurface eglSurface);
    void makeCurrent(EGLSurface eglSurface);
    void swapBuffers(EGLSurface eglSurface);
private:
    bool init(wl_display* waylandDisplay);
    void finish();

    EGLDisplay display = EGL_NO_DISPLAY;
    EGLConfig config = nullptr;
    EGLContext context = EGL_NO_CONTEXT;
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceExt = nullptr;
    PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayExt = nullptr;
};


#endif //SHANGHAI_EGLWAYLANDCONTEXT_H
