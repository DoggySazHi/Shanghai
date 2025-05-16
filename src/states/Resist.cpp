#include "Resist.h"

void Resist::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        animationMachine.setReferenceTime(time);
        shanghai->velocityX = 0;
        shanghai->velocityY = 0;
        lastCurX = state->curX;
        lastCurY = state->curY;
    }

    auto frame = animationMachine.getFrame(time);
    shanghai->setTexture(frame.textureIndex);

    shanghai->flip = false;
    shanghai->positionX = (float) (state->curX - SHANGHAI_TEXTURE_WIDTH / 2.0);
    shanghai->positionY = (float) (state->height - state->curY - SHANGHAI_TEXTURE_WIDTH / 1.2);

    int differenceX = state->curX - lastCurX;
    shanghai->velocityX = alpha * (float) differenceX + (1 - alpha) * shanghai->velocityX;

    int differenceY = state->curY - lastCurY;
    shanghai->velocityY = alpha * (float) differenceY + (1 - alpha) * shanghai->velocityY;

    // Shanghai will no longer resist if shaken
    if (shanghai->velocityX > 1.0f || shanghai->velocityX < -1.0f) {
        machine->setState(ShanghaiState::DRAGGED);
    }

    lastCurX = state->curX;
    lastCurY = state->curY;
}
