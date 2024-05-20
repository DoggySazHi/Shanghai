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
    differenceSmoothed = alpha * (float) difference + (1 - alpha) * differenceSmoothed;

    if (differenceSmoothed > 20) {
        shanghai->setTexture(8);
    } else if (differenceSmoothed > 5) {
        shanghai->setTexture(6);
    } else if (differenceSmoothed > -5) {
        shanghai->setTexture(0);
    } else if (differenceSmoothed > -20) {
        shanghai->setTexture(7);
    } else {
        shanghai->setTexture(9);
    }

    lastCurX = state->curX;
}
