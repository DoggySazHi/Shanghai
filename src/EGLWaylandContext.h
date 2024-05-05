#ifndef SHANGHAI_EGLWAYLANDCONTEXT_H
#define SHANGHAI_EGLWAYLANDCONTEXT_H

#include <wayland-client.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>


class EGLWaylandContext {
public:
    explicit EGLWaylandContext(wl_display* waylandDisplay);
    ~EGLWaylandContext();
private:
    bool init(wl_display* waylandDisplay);
    void finish();

    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC createPlatformWindowSurfaceExt;
    PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplayExt;
};


#endif //SHANGHAI_EGLWAYLANDCONTEXT_H
