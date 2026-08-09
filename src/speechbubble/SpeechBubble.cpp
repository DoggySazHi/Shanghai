#include "SpeechBubble.h"

#include "../Shanghai.h"

uint64_t SpeechBubble::msPerCharacter = 50;
uint64_t SpeechBubble::msExpirationPerCharacter = 100;
int SpeechBubble::margin = 10;

FontRenderer* SpeechBubble::font = nullptr;
NinePatch* SpeechBubble::ninePatch = nullptr;

SpeechBubble::SpeechBubble() {
    if (font == nullptr) {
        font = new FontRenderer("TamzenForPowerline10x20r.bdf");
    }

    if (ninePatch == nullptr) {
        ninePatch = new NinePatch("img/message.png", 12, 12, 12, 12);
    }
}

SpeechBubble::~SpeechBubble() = default;

void SpeechBubble::setText(const std::string& text) {
    this->text = text;
    startTyping = Shanghai::getTime();
    expirationTimer = 0;
    textSize = font->measureString(text);
}
void SpeechBubble::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void SpeechBubble::render(const EGLState* state) {
    uint64_t currentTime = Shanghai::getTime();

    if (expirationTimer != 0 && currentTime > expirationTimer) {
        // Speech bubble expired - do not render
        return;
    }

    uint64_t charactersRendered = (currentTime - startTyping) / msPerCharacter;

    if (charactersRendered >= text.length()) {
        if (expirationTimer == 0 && msExpirationPerCharacter != 0) {
            expirationTimer = currentTime + msExpirationPerCharacter * text.length();
        }
    }

    std::string_view currentSpeech = std::string_view(text).substr(0, charactersRendered);

    ninePatch->render(state, x - textSize.width / 2, y, textSize.width + margin * 2, textSize.height + margin * 2);
    font->renderString(state, currentSpeech, x + margin - textSize.width / 2, y + margin, 0, 0, 0, 255);
}

void SpeechBubble::releaseSharedResources() {
    delete font;
    delete ninePatch;
}
