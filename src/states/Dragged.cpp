#include "Dragged.h"

void Dragged::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        lastCycleTime = time - DRAGGED_STEP_TIME;
    }

    if (time - lastCycleTime > DRAGGED_STEP_TIME) {
        lastCycleTime = time;
    }

    shanghai->flip = false;
    shanghai->positionX = (float) (state->curX - SHANGHAI_TEXTURE_WIDTH / 2.0);
    shanghai->positionY = (float) (state->height - state->curY - SHANGHAI_TEXTURE_WIDTH / 1.2);

    int difference = state->curX - lastCurX;
    shanghai->velocityX = alpha * (float) difference + (1 - alpha) * shanghai->velocityX;

    // Shanghai will resist if dragged and not moving
    if (lastCycleTime == time && shanghai->velocityX < 1.0f && shanghai->velocityX > -1.0f) {
        machine->setState(ShanghaiState::RESIST);
    }

    if (shanghai->velocityX > 20) {
        shanghai->setTexture(8);
    } else if (shanghai->velocityX > 5) {
        shanghai->setTexture(6);
    } else if (shanghai->velocityX > -5) {
        shanghai->setTexture(0);
    } else if (shanghai->velocityX > -20) {
        shanghai->setTexture(7);
    } else {
        shanghai->setTexture(9);
    }

    lastCurX = state->curX;
}
