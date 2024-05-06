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
#include "Shader.h"

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
static uint32_t output = UINT32_MAX;

static uint32_t layer = ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM;
static uint32_t anchor = 0 | ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
static uint32_t width = 128, height = 128;
static int32_t margin_top = 0;
static int32_t margin_bottom = 0;
static bool run_display = true;
static enum zwlr_layer_surface_v1_keyboard_interactivity keyboard_interactive = ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE;
static int cur_x = -1, cur_y = -1;
static int buttons = 0;

struct wl_cursor_image *cursor_image;
struct wl_surface *cursor_surface, *input_surface;

#define TEXTURE_COUNT 46
GLuint textures[TEXTURE_COUNT];
int textureIndex = 0;

EGLWaylandContext* eglWaylandContext;

Shader* shader;

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

    glViewport(0, 0, (int) width, (int) height);
    if (buttons) {
        // Do something, idk
        textureIndex = (textureIndex + 1) % TEXTURE_COUNT;
    }

    glClearColor(0, 0, 0, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (cur_x != -1 && cur_y != -1) {
        glEnable(GL_SCISSOR_TEST);
        glScissor(cur_x, (int) (height - cur_y), 5, 5);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
    }

    shader->use();

    // Packed as [x, y, u, v]
    // Renders a square
    static const GLfloat vertices[] = {
            -1.0, -1.0, 0.0, 0.0,
            1.0, -1.0, 1.0, 0.0,
            1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0,
            -1.0, 1.0, 0.0, 1.0,
            -1.0, -1.0, 0.0, 0.0,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("texture2", 0);
    glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

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
    width = w;
    height = h;

    if (egl_window) {
        wl_egl_window_resize(egl_window, (int) width, (int) height, 0, 0);
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

static void wl_pointer_enter([[maybe_unused]] void *data, struct wl_pointer *wl_pointer, uint32_t serial, struct wl_surface *surface, [[maybe_unused]] wl_fixed_t surface_x, [[maybe_unused]] wl_fixed_t surface_y) {
    struct wl_cursor_image *image;
    image = cursor_image;
    wl_surface_attach(cursor_surface, wl_cursor_image_get_buffer(image), 0, 0);
    wl_surface_damage(cursor_surface, 1, 0, (int) image->width, (int) image->height);
    wl_surface_commit(cursor_surface);
    wl_pointer_set_cursor(wl_pointer, serial, cursor_surface, (int) image->hotspot_x, (int) image->hotspot_y);
    input_surface = surface;
}

static void wl_pointer_leave([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t serial, [[maybe_unused]] struct wl_surface *surface) {
    cur_x = cur_y = -1;
    buttons = 0;
}

static void wl_pointer_motion([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t time, wl_fixed_t surface_x, wl_fixed_t surface_y) {
    cur_x = wl_fixed_to_int(surface_x);
    cur_y = wl_fixed_to_int(surface_y);
}

static void wl_pointer_button([[maybe_unused]] void *data, [[maybe_unused]] struct wl_pointer *wl_pointer, [[maybe_unused]] uint32_t serial, [[maybe_unused]] uint32_t time, [[maybe_unused]] uint32_t button, uint32_t state) {
    if (input_surface == wl_surface) {
        if (state == WL_POINTER_BUTTON_STATE_PRESSED) {
            // button == BTN_RIGHT
            buttons++;
        } else { // On release
            buttons--;
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
    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        compositor = static_cast<wl_compositor *>(wl_registry_bind(registry, name,
                                                                   &wl_compositor_interface, 1));
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        shm = static_cast<wl_shm *>(wl_registry_bind(registry, name,
                                                     &wl_shm_interface, 1));
    } else if (strcmp(interface, "wl_output") == 0) {
        if (output != UINT32_MAX) {
            if (!wl_output) {
                wl_output = static_cast<struct wl_output *>(wl_registry_bind(registry, name,
                                                                      &wl_output_interface, 1));
            } else {
                output--;
            }
        }
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        seat = static_cast<wl_seat *>(wl_registry_bind(registry, name,
                                                       &wl_seat_interface, 1));
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
    zwlr_layer_surface_v1_set_size(layer_surface, width, height);
    zwlr_layer_surface_v1_set_anchor(layer_surface, anchor);
    zwlr_layer_surface_v1_set_exclusive_zone(layer_surface, 0);
    zwlr_layer_surface_v1_set_margin(layer_surface, margin_top, 0, margin_bottom, 0);
    zwlr_layer_surface_v1_set_keyboard_interactivity(layer_surface, keyboard_interactive);
    zwlr_layer_surface_v1_add_listener(layer_surface, &layer_surface_listener, layer_surface);
    wl_surface_commit(wl_surface);
    wl_display_roundtrip(display);

    // Set up XDG surface
    egl_window = wl_egl_window_create(wl_surface, (int) width, (int) height);
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

    shader = new Shader("shader/shanghai.vert", "shader/shanghai.frag");
    if (!shader->isCompiled()) {
        std::cerr << "Failed to compile shader\n";
        return 1;
    }

    std::cout << "Loading textures...\n";

    glGenTextures(TEXTURE_COUNT, textures);
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int imgWidth, imgHeight, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        std::string path = "img/shime" + std::to_string(i + 1) + ".png";
        unsigned char *data = stbi_load(path.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    std::cout << "Loaded " << TEXTURE_COUNT << " textures\n";

    std::cout << "Starting display...\n";

    draw();

    while (wl_display_dispatch(display) != -1 && run_display) {
        // This space intentionally left blank
    }

    wl_cursor_theme_destroy(cursor_theme);
    return 0;
}