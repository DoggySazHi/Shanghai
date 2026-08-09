#ifndef SHANGHAI_SPEECHBUBBLE_H
#define SHANGHAI_SPEECHBUBBLE_H

#include <string>
#include "../gl.h"
#include "../state.h"
#include "../fonts/FontRenderer.h"
#include "../9patch/NinePatch.h"

class SpeechBubble {
public:
    SpeechBubble();
    ~SpeechBubble();

    static uint64_t msPerCharacter;
    static uint64_t msExpirationPerCharacter; // Longer messages take longer to expire
    static uint64_t msExpirationMin; // Minimum time before expiration
    static int margin;

    void setPosition(int x, int y);
    void setText(const std::string& text);
    void render(const EGLState *state);
    static void releaseSharedResources();
private:
    static FontRenderer* font;
    static NinePatch* ninePatch;

    uint64_t startTyping = 0;
    uint64_t expirationTimer = 0;
    std::string text;
    TextSize textSize { .width = 0, .height = 0 };
    int x = 0, y = 0;
};

#endif //SHANGHAI_SPEECHBUBBLE_H
