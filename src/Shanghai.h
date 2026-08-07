#ifndef SHANGHAI_SHANGHAI_H
#define SHANGHAI_SHANGHAI_H


#include <chrono>
#include <vector>
#include "Shader.h"
#include "gl/Quad.h"
#include "state.h"
#include "ShanghaiStateMachine.h"

#ifdef SHANGHAI_PLATFORM_WAYLAND
#include <wayland-client-protocol.h>
#include <wayland-cursor.h>
#elif defined(SHANGHAI_PLATFORM_X11)
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#endif

#define SHANGHAI_TEXTURE_COUNT 46
#define SHANGHAI_TEXTURE_WIDTH 128

// Forward declaration
class ShanghaiStateMachine;

class Shanghai {
public:
    Shanghai();
    ~Shanghai();
    void draw(EGLState* state);
    void setTexture(int index);
    void setScreenGeometry(uint32_t width, uint32_t height);
    static uint64_t getTime();
    [[nodiscard]] ShanghaiStateMachine* getStateMachine() const;

    float positionX = 0, positionY = 0;
    float velocityX = 0, velocityY = 0; // only used by dragging and falling states
    bool flip = true; // false: left, true: right
    [[nodiscard]] bool inShanghai(EGLState* state) const;

    static void updateCursor(const std::vector<Shanghai*>& shanghais, EGLState* state);

    /**
     * Release the textures, shader and geometry shared by every Shanghai. Call
     * once, while the GL context is still current, after the last one is gone.
     */
    static void releaseSharedResources();
private:
#ifdef SHANGHAI_PLATFORM_WAYLAND
    static wl_region *inputRegion;
#elif defined(SHANGHAI_PLATFORM_X11)
    static _XDisplay* xDisplay;
    static unsigned long xWindow;
#endif

    static Shader* shader;
    static Quad* quad;
    static GLuint textures[SHANGHAI_TEXTURE_COUNT];
    int textureIndex = 0;
    uint32_t displayWidth = 0, displayHeight = 0;

    ShanghaiStateMachine* stateMachine;
};


#endif //SHANGHAI_SHANGHAI_H
