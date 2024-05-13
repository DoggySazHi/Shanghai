#include <iostream>

#include <cassert>
#include <GLES2/gl2.h>
#include <cstring>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-egl.h>
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "xdg-shell-client-protocol.h"
#include "EGLWaylandContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shanghai.h"
#include "keyboard.h"
#include "pointer.h"
#include "state.h"

// All Wayland runtime variables
static struct wl_display *display;
static struct wl_compositor *compositor;
static struct wl_seat *seat;
static struct wl_shm *shm;
static struct wl_pointer *pointer;
static struct wl_keyboard *keyboard;
static struct xdg_wm_base *xdg_wm_base;
static struct zwlr_layer_shell_v1 *layer_shell;

struct zwlr_layer_surface_v1 *layer_surface;
static struct wl_output *wl_output;

struct wl_surface *wl_surface;
struct wl_egl_window *egl_window;
struct wlr_egl_surface *egl_surface;
struct wl_callback *frame_callback;
// Set to UINT32_MAX to indicate that we don't care about the output (usually last-used monitor)
// Other options: 0 (for first monitor), 1 (for second monitor), 2, etc.
static uint32_t output = UINT32_MAX;

static uint32_t layer = ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM;
static uint32_t anchor = 0 | ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
 // Wayland will resize to full screen, so just keep it zero
static int32_t margin_top = 0;
static int32_t margin_bottom = 0;
static bool run_display = true;
static enum zwlr_layer_surface_v1_keyboard_interactivity keyboard_interactive = ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE;

struct wl_cursor_image *cursor_image;
struct wl_surface *cursor_surface, *input_surface;

// Stuff we care about
EGLWaylandContext* eglWaylandContext;
EGLState eglState;
Shanghai* shanghai;

static void draw();

static void surface_frame_callback([[maybe_unused]] void *data, struct wl_callback *cb, [[maybe_unused]] uint32_t time) {
    wl_callback_destroy(cb);
    frame_callback = nullptr;
    draw();
}

static struct wl_callback_listener frame_listener = {
        .done = surface_frame_callback
};

static void draw() {
    eglWaylandContext->makeCurrent(egl_surface);

    glViewport(0, 0, (int) eglState.width, (int) eglState.height);

    glClearColor(0, 0, 0, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (eglState.curX != -1 && eglState.curY != -1) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(eglState.curX, (int) (eglState.height - eglState.curY), 5, 5);
        glClearColor(0, 0, 0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
    }

    shanghai->draw(&eglState);

    frame_callback = wl_surface_frame(wl_surface);
    wl_callback_add_listener(frame_callback, &frame_listener, nullptr);

    eglWaylandContext->swapBuffers(egl_surface);
}

static void xdg_surface_handle_configure([[maybe_unused]] void *data, struct xdg_surface *xdg_surface, uint32_t serial) {
    xdg_surface_ack_configure(xdg_surface, serial);
}

static const struct xdg_surface_listener xdg_surface_listener = {
        .configure = xdg_surface_handle_configure,
};

static void layer_surface_configure([[maybe_unused]] void *data, struct zwlr_layer_surface_v1 *surface, uint32_t serial, uint32_t w, uint32_t h) {
    eglState.width = w;
    eglState.height = h;

    if (shanghai != nullptr) {
        shanghai->setScreenGeometry(eglState.width, eglState.height);
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

static void handle_global([[maybe_unused]] void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version) {
    std::cout << interface << '\n';
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

int main() {
    const char* wlrNamespace = "wlroots";

    display = wl_display_connect(nullptr);
    if (display == nullptr) {
        fprintf(stderr, "Failed to create display\n");
        return 1;
    }

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, nullptr);
    wl_display_roundtrip(display);

    if (compositor == nullptr) {
        std::cerr << "wl_compositor not available; are you using X11?\n";
        return 1;
    }
    if (shm == nullptr) {
        std::cerr << "wl_shm not available; are you using X11?\n";
        return 1;
    }
    if (layer_shell == nullptr) {
        std::cerr << "layer_shell not available; cannot render without layering support\n";
        return 1;
    }

    // Cursor when you hover over Shanghai
    struct wl_cursor_theme *cursor_theme = wl_cursor_theme_load(nullptr, 16, shm);
    assert(cursor_theme);
    struct wl_cursor *cursor = wl_cursor_theme_get_cursor(cursor_theme, "pointer");
    if (cursor == nullptr) {
        cursor = wl_cursor_theme_get_cursor(cursor_theme, "left_ptr");
    }
    assert(cursor);
    cursor_image = cursor->images[0];

    cursor_surface = wl_compositor_create_surface(compositor);
    assert(cursor_surface);

    // EGL renderer for Shanghai
    eglWaylandContext = new EGLWaylandContext(display);

    wl_surface = wl_compositor_create_surface(compositor);
    assert(wl_surface);

    // Set layer properties
    layer_surface = zwlr_layer_shell_v1_get_layer_surface(layer_shell, wl_surface, wl_output, layer, wlrNamespace);
    assert(layer_surface);
    zwlr_layer_surface_v1_set_size(layer_surface, eglState.width, eglState.height);
    zwlr_layer_surface_v1_set_anchor(layer_surface, anchor);
    zwlr_layer_surface_v1_set_exclusive_zone(layer_surface, 0);
    zwlr_layer_surface_v1_set_margin(layer_surface, margin_top, 0, margin_bottom, 0);
    zwlr_layer_surface_v1_set_keyboard_interactivity(layer_surface, keyboard_interactive);
    zwlr_layer_surface_v1_add_listener(layer_surface, &layer_surface_listener, layer_surface);
    wl_surface_commit(wl_surface);
    wl_display_roundtrip(display);

    // Set up XDG surface
    egl_window = wl_egl_window_create(wl_surface, (int) eglState.width, (int) eglState.height);
    assert(egl_window);
    egl_surface = static_cast<wlr_egl_surface*>(eglWaylandContext->createSurface(egl_window));
    assert(egl_surface != EGL_NO_SURFACE);

    wl_display_roundtrip(display);

    // Preparing our GL context
    eglWaylandContext->makeCurrent(egl_surface);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
    std::cout << "OpenGL renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL vendor: " << glGetString(GL_VENDOR) << '\n';
    std::cout << "OpenGL shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n\n";

    shanghai = new Shanghai();

    std::cout << "Starting display...\n";

    draw();

    while (wl_display_dispatch(display) != -1 && run_display) {
        // This space intentionally left blank
    }

    wl_cursor_theme_destroy(cursor_theme);
    return 0;
}