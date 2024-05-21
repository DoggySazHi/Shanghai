#ifndef SHANGHAI_SEAT_H
#define SHANGHAI_SEAT_H

#include <wayland-client-protocol.h>
#include "pointer.h"
#include "keyboard.h"

struct wl_pointer *pointer;
struct wl_keyboard *keyboard;

static void seat_handle_capabilities([[maybe_unused]] void *data, struct wl_seat *wl_seat, uint32_t caps) {
    if ((caps & WL_SEAT_CAPABILITY_POINTER)) {
        pointer = wl_seat_get_pointer(wl_seat);
        wl_pointer_add_listener(pointer, &pointer_listener, nullptr);
    }

    if ((caps & WL_SEAT_CAPABILITY_KEYBOARD)) {
        keyboard = wl_seat_get_keyboard(wl_seat);
        wl_keyboard_add_listener(keyboard, &keyboard_listener, nullptr);
    }
}

static void seat_handle_name([[maybe_unused]] void *data, [[maybe_unused]] struct wl_seat *wl_seat, [[maybe_unused]] const char *name) {
    // Who cares
}

const struct wl_seat_listener seat_listener = {
        .capabilities = seat_handle_capabilities,
        .name = seat_handle_name,
};


#endif //SHANGHAI_SEAT_H
