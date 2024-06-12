#include <stdexcept>

#include "Shanghai.h"
#include "stb_image.h"

GLuint Shanghai::textures[SHANGHAI_TEXTURE_COUNT] = {0};
#ifdef __WAYLAND__
extern struct wl_compositor *compositor;
extern struct wl_surface *wl_surface, *cursor_surface;
extern struct wl_cursor* left_ptr_cursor;
extern struct wl_cursor* pointer_cursor;
wl_region* Shanghai::inputRegion = nullptr;
#elif __X11__
extern struct GLFWwindow* glfwWindow;
_XDisplay* Shanghai::xDisplay = nullptr;
unsigned long Shanghai::xWindow = 0;
#endif
Shader* Shanghai::shader = nullptr;

Shanghai::Shanghai() {
    if (shader == nullptr) {
        shader = new Shader("shader/shanghai.vert", "shader/shanghai.frag");

        if (!shader->isCompiled()) {
            throw std::runtime_error("Failed to compile shader");
        }
    }

#ifdef __WAYLAND__
    if (inputRegion == nullptr) {
        inputRegion = wl_compositor_create_region(compositor);
    }
#elif __X11__
//    if (xDisplay == nullptr) {
//        xDisplay = glfwGetX11Display();
//        xWindow = glfwGetX11Window(glfwWindow);
//        auto xServerRegion = XFixesCreateRegion(xDisplay, &inputRegion, 1);
//        XFixesSetWindowShapeRegion(xDisplay, xWindow, ShapeInput, 0, 0, xServerRegion);
//        XFixesDestroyRegion(xDisplay, xServerRegion);
//    }
#endif

    // Generate textures
    if (textures[0] == 0) {
        glGenTextures(SHANGHAI_TEXTURE_COUNT, textures);
        for (int i = 0; i < SHANGHAI_TEXTURE_COUNT; i++) {
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
                throw std::runtime_error("Failed to load texture " + path);
            }
            stbi_image_free(data);
        }
    }

    stateMachine = new ShanghaiStateMachine();
}

Shanghai::~Shanghai() {
    delete shader;
    glDeleteTextures(SHANGHAI_TEXTURE_COUNT, textures);
    delete stateMachine;
}

/**
 * Update the Wayland cursor image based on the current state.
 * @param state The current EGL state (mouse position)
 */
void Shanghai::updateCursor(const std::vector<Shanghai*>& shanghais, EGLState* state) {
#ifdef __WAYLAND__
    struct wl_cursor_image *image;

    auto time = getTime();

    // Check if mouse is in bounding box of Shanghai
    wl_cursor* cursor = left_ptr_cursor;
    wl_region_destroy(inputRegion);
    inputRegion = wl_compositor_create_region(compositor);

    bool inAnyShanghai = false;
    for (auto shanghai : shanghais) {
        // Check if the cursor is in any Shanghai
        if (shanghai->inShanghai(state)) {
            inAnyShanghai = true;
        }

        // Allow the cursor to be over any Shanghais (allows them to be yeeted)
        wl_region_add(inputRegion, (int) shanghai->positionX, (int) (state->height - (int) shanghai->positionY - SHANGHAI_TEXTURE_WIDTH), SHANGHAI_TEXTURE_WIDTH, SHANGHAI_TEXTURE_WIDTH);
    }

    wl_surface_set_input_region(wl_surface, inputRegion);

    if (inAnyShanghai) {
        cursor = pointer_cursor;

        if (!state->inShanghai) {
            state->inShanghai = true;
            state->cursorAnimationTime = time;
        }
    } else if (state->inShanghai) {
        state->inShanghai = false;
        state->cursorAnimationTime = time;
    }

    image = cursor->images[wl_cursor_frame(cursor, time - state->cursorAnimationTime)];
    wl_surface_attach(cursor_surface, wl_cursor_image_get_buffer(image), 0, 0);
    wl_surface_damage(cursor_surface, 1, 0, (int) image->width, (int) image->height);
    wl_surface_commit(cursor_surface);
#elif __X11__
    auto region = XCreateRegion();

    XRectangle rect;
    rect.width = SHANGHAI_TEXTURE_WIDTH;
    rect.height = SHANGHAI_TEXTURE_WIDTH;

    for (auto shanghai : shanghais) {
        // Allow the cursor to be over any Shanghais (allows them to be yeeted)
        rect.x = (short) shanghai->positionX;
        rect.y = (short) (state->height - (short) shanghai->positionY - SHANGHAI_TEXTURE_WIDTH);
        XUnionRectWithRegion(&rect, region, region);
    }

    XShapeCombineRegion(xDisplay, xWindow, ShapeInput, 0, 0, region, ShapeSet);
    XDestroyRegion(region);
#endif
}

void Shanghai::draw(EGLState* state) {
    stateMachine->frame(this, state);

    shader->use();

    // Packed as [x, y, u, v]
    // Renders a square
    static const GLfloat vertices[] = {
            0, 0, 0.0, 0.0,
            0, 128, 0.0, 1.0,
            128, 0, 1.0, 0.0,
            128, 128, 1.0, 1.0,
    };

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), vertices + 2);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("shanghaiTexture", 0);
    shader->setUniform("screenGeometry", (float) displayWidth, (float) displayHeight);
    shader->setUniform("shanghaiPosition", positionX, positionY);
    shader->setUniform("shanghaiHFlip", flip);
    glBindTexture(GL_TEXTURE_2D, textures[textureIndex]);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Shanghai::setScreenGeometry(uint32_t width, uint32_t height) {
    displayWidth = width;
    displayHeight = height;
}

/**
 * Select the texture file to render.
 * Note that the texture index is 0-based; shime1.png is index 0, shime2.png is index 1, etc.
 * @param index The 0-based index of the texture to render
 */
void Shanghai::setTexture(int index) {
    if (index < 0 || index >= SHANGHAI_TEXTURE_COUNT) {
        throw std::runtime_error("Invalid texture index " + std::to_string(index) + " out of bounds 0-" + std::to_string(SHANGHAI_TEXTURE_COUNT - 1) + ".");
    }

    textureIndex = index;
}

/**
 * Get the current time in milliseconds. This uses a monotonic clock.
 * @return The current time in milliseconds
 */
uint64_t Shanghai::getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool Shanghai::inShanghai(EGLState* state) const {
    return (float) state->curX > positionX &&
           (float) state->curX < positionX + SHANGHAI_TEXTURE_WIDTH &&
           (float) (state->height - state->curY) > positionY &&
           (float) (state->height - state->curY) < positionY + SHANGHAI_TEXTURE_WIDTH;
}

ShanghaiStateMachine *Shanghai::getStateMachine() const {
    return stateMachine;
}
