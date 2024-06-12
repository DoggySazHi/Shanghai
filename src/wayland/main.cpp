#include <iostream>

#include <cassert>
#include "../gl.h"
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-egl.h>
#include "wlr-layer-shell-unstable-v1-client-protocol.h"
#include "EGLWaylandContext.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../Shanghai.h"
#include "../config/ShanghaiConfiguration.h"
#include "../Background.h"
#include "registry.h"
#include "layer.h"
#include "../Random.h"


// All Wayland runtime variables
static struct wl_display *display;
struct wl_compositor *compositor;
struct wl_shm *shm;
struct xdg_wm_base *xdg_wm_base;
struct zwlr_layer_shell_v1 *layer_shell;

struct zwlr_layer_surface_v1 *layer_surface;
struct wl_output *wl_output;

struct wl_surface *wl_surface;
struct wl_egl_window *egl_window;
struct wlr_egl_surface *egl_surface;
struct wl_callback *frame_callback;
// Set to UINT32_MAX to indicate that we don't care about the output (usually last-used monitor)
// Other options: 0 (for first monitor), 1 (for second monitor), 2, etc.
uint32_t output = UINT32_MAX;

static uint32_t layer = ZWLR_LAYER_SHELL_V1_LAYER_BOTTOM;
static uint32_t anchor = 0 | ZWLR_LAYER_SURFACE_V1_ANCHOR_TOP | ZWLR_LAYER_SURFACE_V1_ANCHOR_LEFT | ZWLR_LAYER_SURFACE_V1_ANCHOR_RIGHT | ZWLR_LAYER_SURFACE_V1_ANCHOR_BOTTOM;
 // Wayland will resize to full screen, so just keep it zero
static int32_t margin_top = 0;
static int32_t margin_bottom = 0;
bool run_display = true;
static enum zwlr_layer_surface_v1_keyboard_interactivity keyboard_interactive = ZWLR_LAYER_SURFACE_V1_KEYBOARD_INTERACTIVITY_NONE;

struct wl_cursor* left_ptr_cursor;
struct wl_cursor* pointer_cursor;
struct wl_surface *cursor_surface, *input_surface;

// Stuff we care about
EGLWaylandContext* eglWaylandContext;
EGLState eglState;
ShanghaiConfiguration* config;
Background* background;
std::vector<Shanghai*> shanghais;

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

    if (config->isBackgroundEnabled()) {
        background->draw(&eglState);
    }

    for (auto& shanghai : shanghais) {
        shanghai->draw(&eglState);
    }

    Shanghai::updateCursor(shanghais, &eglState);

    frame_callback = wl_surface_frame(wl_surface);
    wl_callback_add_listener(frame_callback, &frame_listener, nullptr);

    eglWaylandContext->swapBuffers(egl_surface);
}

int main() {
    const char* wlrNamespace = "wlroots";

    std::cout << "Loading configuration...\n";
    config = ShanghaiConfiguration::getInstance();
    output = config->getOutput();
    layer = config->getLayer();

    display = wl_display_connect(nullptr);
    if (display == nullptr) {
        fprintf(stderr, "Failed to create output\n");
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

    left_ptr_cursor = wl_cursor_theme_get_cursor(cursor_theme, "left_ptr");
    pointer_cursor = wl_cursor_theme_get_cursor(cursor_theme, "pointer");
    if (pointer_cursor == nullptr) {
        pointer_cursor = left_ptr_cursor;
    }
    assert(left_ptr_cursor);
    assert(pointer_cursor);

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

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (config->isBackgroundEnabled()) {
        background = new Background();
    }

    shanghais.push_back(new Shanghai());
    ShanghaiState states[] = {ShanghaiState::CRAWLING, ShanghaiState::SITTING_AND_LOOKING, ShanghaiState::SITTING, ShanghaiState::WALKING, ShanghaiState::JUMP};
//    ShanghaiState states[] = {ShanghaiState::WALL_HOLD, ShanghaiState::WALL_CLIMB};
    for (int i = 0; i < 40; ++i) {
        auto* shanghai = new Shanghai();
        shanghai->positionX = i * 128;
        shanghais.push_back(shanghai);

        shanghai->getStateMachine()->setState(states[(int) (Random::rand() * std::size(states))]);
        shanghai->flip = Random::rand() < 0.5;
    }

    std::cout << "Starting output...\n";

    draw();

    while (wl_display_dispatch(display) != -1 && run_display) {
        // This space intentionally left blank
    }

    wl_cursor_theme_destroy(cursor_theme);
    return 0;
}