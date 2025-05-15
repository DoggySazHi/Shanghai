#include "Resist.h"

void Resist::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        animationMachine.setReferenceTime(time);
        shanghai->velocityX = 0;
        lastCurX = state->curX;
    }

    auto frame = animationMachine.getFrame(time);
    shanghai->setTexture(frame.textureIndex);

    shanghai->flip = false;
    shanghai->positionX = (float) (state->curX - SHANGHAI_TEXTURE_WIDTH / 2.0);
    shanghai->positionY = (float) (state->height - state->curY - SHANGHAI_TEXTURE_WIDTH / 1.2);

    int difference = state->curX - lastCurX;
    shanghai->velocityX = alpha * (float) difference + (1 - alpha) * shanghai->velocityX;

    // Shanghai will no longer resist if shaken
    if (shanghai->velocityX > 1.0f || shanghai->velocityX < -1.0f) {
        machine->setState(ShanghaiState::DRAGGED);
    }

    lastCurX = state->curX;
}
