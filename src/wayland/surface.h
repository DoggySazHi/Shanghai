#ifndef SHANGHAI_SURFACE_H
#define SHANGHAI_SURFACE_H

#include "xdg-shell-client-protocol.h"

static void xdg_surface_handle_configure([[maybe_unused]] void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);
}

static const struct xdg_surface_listener xdg_surface_listener = {
        .configure = xdg_surface_handle_configure,
};

#endif //SHANGHAI_SURFACE_H
