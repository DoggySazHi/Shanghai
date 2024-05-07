#ifndef SHANGHAI_POINTER_H
#define SHANGHAI_POINTER_H

#include <wayland-client-protocol.h>
#include <wayland-cursor.h>
#include "state.h"

extern struct wl_cursor_image *cursor_image;
extern struct wl_surface *wl_surface;
extern struct wl_surface *cursor_surface, *input_surface;
extern EGLState eglState;

static void wl_pointer_enter([[maybe_unused]] void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, [[maybe_unused]] wl_fixed_t surface_x, [[maybe_unused]] wl_fixed_t surface_y) {
    struct wl_cursor_image *image;
    image = cursor_image;
    wl_surface_attach(cursor_surface, wl_cursor_image_get_buffer(image), 0, 0);
    wl_surface_damage(cursor_surface, 1, 0, (int) image->width, (int) image->height);
    wl_surface_commit(cursor_surface);
    wl_pointer_set_cursor(wl_pointer, serial, cursor_surface, (int) image->hotspot_x, (int) image->hotspot_y);
    input_surface = surface;

#ifdef DEBUG
    std::cout << "Entered surface" << std::endl;
#endif
}

static void wl_pointer_leave([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t serial, [[maybe_unused]] struct wl_surface *surface) {
    eglState.curX = eglState.curY = -1;
    eglState.buttons = 0;

#ifdef DEBUG
    std::cout << "Left surface" << std::endl;
#endif
}

static void wl_pointer_motion([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    eglState.curX = wl_fixed_to_int(surface_x);
    eglState.curY = wl_fixed_to_int(surface_y);
}

static void wl_pointer_button([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t serial, [[maybe_unused]] uint32_t time, [[maybe_unused]] uint32_t button, uint32_t state) {
    if (input_surface == wl_surface) {
        if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
            // button == BTN_RIGHT
            eglState.buttons++;
        } else { // On release
            eglState.buttons--;
        }
    } else {
        assert(false && "Unknown surface");
    }
}

static void wl_pointer_axis([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t time, [[maybe_unused]] uint32_t axis, [[maybe_unused]] wl_fixed_t value) {
    // Unused
}

static void wl_pointer_frame([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer) {
    // Unused
}

static void wl_pointer_axis_source([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t axis_source) {
    // Unused
}

static void wl_pointer_axis_stop([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t time, [[maybe_unused]] uint32_t axis) {
    // Unused
}

static void wl_pointer_axis_discrete([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t axis, [[maybe_unused]] int32_t discrete) {
    // Unused
}

struct wl_pointer_listener pointer_listener = {
        .enter = wl_pointer_enter,
        .leave = wl_pointer_leave,
        .motion = wl_pointer_motion,
        .button = wl_pointer_button,
        .axis = wl_pointer_axis,
        .frame = wl_pointer_frame,
        .axis_source = wl_pointer_axis_source,
        .axis_stop = wl_pointer_axis_stop,
        .axis_discrete = wl_pointer_axis_discrete,
};

#endif //SHANGHAI_POINTER_H
