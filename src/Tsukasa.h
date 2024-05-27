#ifndef Tsukasa_TSUKASA_H
#define Tsukasa_TSUKASA_H


#include <chrono>
#include "Shader.h"
#include "state.h"
#include "animation/ShanghaiAnimationMachine.h"

#ifdef __WAYLAND__
#include <wayland-client-protocol.h>
#include <wayland-cursor.h>
#endif

#define TSUKASA_TEXTURE_COUNT 48
#define TSUKASA_TEXTURE_WIDTH 212
#define TSUKASA_TEXTURE_HEIGHT 204

class Tsukasa {
public:
    Tsukasa();
    ~Tsukasa();
    void draw(EGLState* state);
    void setTexture(int index);
    void setScreenGeometry(uint32_t width, uint32_t height);
    static uint64_t getTime();

    float positionX = 0, positionY = 0;
    bool flip = true;
    [[nodiscard]] bool inTsukasa(EGLState* state) const;

    static void updateCursor(const std::vector<Tsukasa*>& Tsukasas, EGLState* state);
private:
#ifdef __WAYLAND__
    static wl_region *inputRegion;
#endif

    static Shader* shader;
    static GLuint textures[TSUKASA_TEXTURE_COUNT];
    int textureIndex = 0;
    uint32_t displayWidth = 0, displayHeight = 0;

    ShanghaiAnimationMachine animation = ShanghaiAnimationMachine({
        {0, 0, 0, 0},
    });
};


#endif //Tsukasa_TSUKASA_H
