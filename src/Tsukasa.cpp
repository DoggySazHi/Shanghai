#include <stdexcept>

#include "Tsukasa.h"
#include "stb_image.h"

extern struct wl_compositor *compositor;
extern struct wl_surface *wl_surface, *cursor_surface;
extern struct wl_cursor* left_ptr_cursor;
extern struct wl_cursor* pointer_cursor;

GLuint Tsukasa::textures[TSUKASA_TEXTURE_COUNT] = {0};
wl_region* Tsukasa::inputRegion = nullptr;
Shader* Tsukasa::shader = nullptr;

Tsukasa::Tsukasa() {
    if (shader == nullptr) {
        shader = new Shader("shader/shanghai.vert", "shader/shanghai.frag");

        if (!shader->isCompiled()) {
            throw std::runtime_error("Failed to compile shader");
        }
    }

    if (inputRegion == nullptr) {
        inputRegion = wl_compositor_create_region(compositor);
    }

    // Generate textures
    if (textures[0] == 0) {
        glGenTextures(TSUKASA_TEXTURE_COUNT, textures);
        for (int i = 0; i < TSUKASA_TEXTURE_COUNT; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            int imgWidth, imgHeight, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            std::string path = "img/tsukasa" + std::to_string(i + 1) + ".png";
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

    std::vector<ShanghaiAnimationFrame> frames;
    for (int i = 0; i < TSUKASA_TEXTURE_COUNT; i++) {
        frames.emplace_back(i, 1, 1, 0);
    }
    animation = ShanghaiAnimationMachine(frames);
    animation.setReferenceTime(0);
    animation.setAnimationFactor(12x2);
}

Tsukasa::~Tsukasa() {
    delete shader;
    glDeleteTextures(TSUKASA_TEXTURE_COUNT, textures);
}

/**
 * Update the Wayland cursor image based on the current state.
 * @param state The current EGL state (mouse position)
 */
void Tsukasa::updateCursor(const std::vector<Tsukasa*>& shanghais, EGLState* state) {
#ifdef __WAYLAND__
    struct wl_cursor_image *image;

    auto time = getTime();

    // Check if mouse is in bounding box of Tsukasa
    wl_cursor* cursor = left_ptr_cursor;
    wl_region_destroy(inputRegion);
    inputRegion = wl_compositor_create_region(compositor);

    bool inAnyTsukasa = false;
    for (auto shanghai : shanghais) {
        // Check if the cursor is in any Tsukasa
        if (shanghai->inTsukasa(state)) {
            inAnyTsukasa = true;
        }

        // Allow the cursor to be over any Tsukasas (allows them to be yeeted)
        wl_region_add(inputRegion, (int) shanghai->positionX, (int) (state->height - (int) shanghai->positionY - TSUKASA_TEXTURE_HEIGHT), TSUKASA_TEXTURE_WIDTH, TSUKASA_TEXTURE_HEIGHT);
    }

    wl_surface_set_input_region(wl_surface, inputRegion);

    if (inAnyTsukasa) {
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
#endif
}

void Tsukasa::draw(EGLState* state) {
    auto frame = animation.getFrame(getTime());
    setTexture(frame.textureIndex);
    positionX += frame.translationX;

    shader->use();

    // Packed as [x, y, u, v]
    // Renders a square
    static const GLfloat vertices[] = {
            0, 0, 0.0, 0.0,
            0, TSUKASA_TEXTURE_HEIGHT, 0.0, 1.0,
            TSUKASA_TEXTURE_WIDTH, 0, 1.0, 0.0,
            TSUKASA_TEXTURE_WIDTH, TSUKASA_TEXTURE_HEIGHT, 1.0, 1.0,
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

void Tsukasa::setScreenGeometry(uint32_t width, uint32_t height) {
    displayWidth = width;
    displayHeight = height;
}

/**
 * Select the texture file to render.
 * Note that the texture index is 0-based; shime1.png is index 0, shime2.png is index 1, etc.
 * @param index The 0-based index of the texture to render
 */
void Tsukasa::setTexture(int index) {
    if (index < 0 || index >= TSUKASA_TEXTURE_COUNT) {
        throw std::runtime_error("Invalid texture index " + std::to_string(index) + " out of bounds 0-" + std::to_string(TSUKASA_TEXTURE_COUNT - 1) + ".");
    }

    textureIndex = index;
}

/**
 * Get the current time in milliseconds. This uses a monotonic clock.
 * @return The current time in milliseconds
 */
uint64_t Tsukasa::getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

bool Tsukasa::inTsukasa(EGLState* state) const {
    return (float) state->curX > positionX &&
           (float) state->curX < positionX + TSUKASA_TEXTURE_WIDTH &&
           (float) (state->height - state->curY) > positionY &&
           (float) (state->height - state->curY) < positionY + TSUKASA_TEXTURE_HEIGHT;
}
