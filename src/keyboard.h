#ifndef SHANGHAI_KEYBOARD_H
#define SHANGHAI_KEYBOARD_H

#include <wayland-client-protocol.h>

static void wl_keyboard_keymap([[maybe_unused]] void *data, [[maybe_unused]] struct wl_keyboard *wl_keyboard, [[maybe_unused]] uint32_t format, [[maybe_unused]] int32_t fd, [[maybe_unused]] uint32_t size) {
    // Who cares
}

static void wl_keyboard_enter([[maybe_unused]] void *data, [[maybe_unused]] struct wl_keyboard *wl_keyboard, [[maybe_unused]] uint32_t serial, [[maybe_unused]] struct wl_surface *surface, [[maybe_unused]] struct wl_array *keys) {
    // Who cares
}

static void wl_keyboard_leave([[maybe_unused]] void *data, [[maybe_unused]] struct wl_keyboard *wl_keyboard, [[maybe_unused]] uint32_t serial, [[maybe_unused]] struct wl_surface *surface) {
    // Who cares
}

static void wl_keyboard_key([[maybe_unused]] void *data, [[maybe_unused]] struct wl_keyboard *wl_keyboard, [[maybe_unused]] uint32_t serial, [[maybe_unused]] uint32_t time, [[maybe_unused]] uint32_t key, [[maybe_unused]] uint32_t state) {
    // Actually might be useful
}

static void wl_keyboard_modifiers([[maybe_unused]] void *data, [[maybe_unused]] struct wl_keyboard *wl_keyboard, [[maybe_unused]] uint32_t serial, [[maybe_unused]] uint32_t mods_depressed, [[maybe_unused]] uint32_t mods_latched, [[maybe_unused]] uint32_t mods_locked, [[maybe_unused]] uint32_t group) {
    // Who cares
}

static void wl_keyboard_repeat_info([[maybe_unused]] void *data, [[maybe_unused]] struct wl_keyboard *wl_keyboard, [[maybe_unused]] int32_t rate, [[maybe_unused]] int32_t delay) {
    // Who cares
}

static struct wl_keyboard_listener keyboard_listener = {
        .keymap = wl_keyboard_keymap,
        .enter = wl_keyboard_enter,
        .leave = wl_keyboard_leave,
        .key = wl_keyboard_key,
        .modifiers = wl_keyboard_modifiers,
        .repeat_info = wl_keyboard_repeat_info,
};

#endif //SHANGHAI_KEYBOARD_H
