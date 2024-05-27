#ifndef SHANGHAI_LAYER_H
#define SHANGHAI_LAYER_H

#include <cstdint>
#include <vector>
#include <wayland-egl.h>
#include <wayland-client.h>

#include "../state.h"
#include "../Shanghai.h"
#include "../Background.h"
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "EGLWaylandContext.h"
#include "../Tsukasa.h"

extern struct wl_egl_window *egl_window;
extern struct wlr_egl_surface *egl_surface;

extern bool run_display;
extern EGLWaylandContext* eglWaylandContext;
extern std::vector<Shanghai*> shanghais;
extern Tsukasa* tsukasa;
extern Background* background;

static void layer_surface_configure([[maybe_unused]] void *data, struct zwlr_layer_surface_v1 *surface, uint32_t serial, uint32_t w, uint32_t h) {
    eglState.width = w;
    eglState.height = h;

    for (auto& shanghai : shanghais) {
        shanghai->setScreenGeometry(w, h);
    }

    if (background != nullptr) {
        background->setScreenGeometry(eglState.width, eglState.height);
    }

    if (tsukasa != nullptr) {
        tsukasa->setScreenGeometry(eglState.width, eglState.height);
    }

    if (egl_window) {
        wl_egl_window_resize(egl_window, (int) eglState.width, (int) eglState.height, 0, 0);
    }

    zwlr_layer_surface_v1_ack_configure(surface, serial);
}

static void layer_surface_closed([[maybe_unused]] void *data, struct zwlr_layer_surface_v1 *surface) {
    eglWaylandContext->destroySurface(egl_surface);
    wl_egl_window_destroy(egl_window);
    zwlr_layer_surface_v1_destroy(surface);
    wl_surface_destroy(wl_surface);
    run_display = false;
}

struct zwlr_layer_surface_v1_listener layer_surface_listener = {
        .configure = layer_surface_configure,
        .closed = layer_surface_closed,
};

#endif //SHANGHAI_LAYER_H
