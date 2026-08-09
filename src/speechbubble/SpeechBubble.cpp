#include "SpeechBubble.h"

#include "../Shanghai.h"

uint64_t SpeechBubble::msPerCharacter = 100;
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

    x = 0, y = 0;
    textSize = { .width = 0, .height = 0 };
    startTyping = false;
}

SpeechBubble::~SpeechBubble() = default;

void SpeechBubble::setText(const std::string& text) {
    this->text = text;
    startTyping = Shanghai::getTime();
    textSize = font->measureString(text);
}
void SpeechBubble::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void SpeechBubble::render(const EGLState* state) const {
    uint64_t currentTime = Shanghai::getTime();
    std::string_view currentSpeech = std::string_view(text).substr(0, (currentTime - startTyping) / msPerCharacter);

    ninePatch->render(state, x - textSize.width / 2, y, textSize.width + margin * 2, textSize.height + margin * 2);
    font->renderString(state, currentSpeech, x + margin - textSize.width / 2, y + margin, 0, 0, 0, 255);
}

void SpeechBubble::releaseSharedResources() {
    delete font;
    delete ninePatch;
}
