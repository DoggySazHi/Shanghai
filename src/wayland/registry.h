#ifndef SHANGHAI_REGISTRY_H
#define SHANGHAI_REGISTRY_H

#include <cstdint>
#include <wayland-client.h>
#include <cstring>
#include "seat.h"
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "xdg-shell-client-protocol.h"

extern struct wl_compositor *compositor;
extern struct wl_shm *shm;
extern struct wl_output *wl_output;
extern struct zwlr_layer_shell_v1 *layer_shell;
extern struct xdg_wm_base *xdg_wm_base;

extern uint32_t output;
struct wl_seat *seat;

static void handle_global([[maybe_unused]] void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        compositor = static_cast<wl_compositor *>(wl_registry_bind(registry, name, &wl_compositor_interface, 1));
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        shm = static_cast<wl_shm *>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
    } else if (strcmp(interface, "wl_output") == 0) {
        if (output != UINT32_MAX) {
            wl_output = static_cast<struct wl_output *>(wl_registry_bind(registry, name, &wl_output_interface, 1));
            output--; // We abuse unsigned integer underflow to stop at the desired output
        }
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        seat = static_cast<wl_seat *>(wl_registry_bind(registry, name, &wl_seat_interface, 1));
        wl_seat_add_listener(seat, &seat_listener, nullptr);
    } else if (strcmp(interface, zwlr_layer_shell_v1_interface.name) == 0) {
        layer_shell = static_cast<zwlr_layer_shell_v1 *>(wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, version < 4 ? version : 4));
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        xdg_wm_base = static_cast<struct xdg_wm_base *>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
    }
}

static void handle_global_remove([[maybe_unused]] void *data, [[maybe_unused]] struct wl_registry *registry, [[maybe_unused]] uint32_t name) {
    // who cares
}

static const struct wl_registry_listener registry_listener = {
        .global = handle_global,
        .global_remove = handle_global_remove,
};

#endif //SHANGHAI_REGISTRY_H
