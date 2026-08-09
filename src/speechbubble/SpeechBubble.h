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
    static int margin;

    void setPosition(int x, int y);
    void setText(const std::string& text);
    void render(const EGLState *state) const;
    static void releaseSharedResources();
private:
    static FontRenderer* font;
    static NinePatch* ninePatch;

    uint64_t startTyping;
    std::string text;
    TextSize textSize{};
    int x, y;
};

#endif //SHANGHAI_SPEECHBUBBLE_H
