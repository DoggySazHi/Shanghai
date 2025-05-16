#include "Jump.h"

void Jump::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    if (machine->isNewStateFlag()) {
        velocity = JUMP_INITIAL_VELOCITY;
    }

    shanghai->setTexture(21);
    shanghai->positionY += velocity;
    velocity -= JUMP_STEP_ACC;

    if (shanghai->positionY < 0) {
        shanghai->positionY = 0;
        machine->setState(ShanghaiState::STANDING);
    }

    // Check if mouse is in bounding box of Shanghai
    if (shanghai->inShanghai(state)) {
        machine->stealCursor(shanghai);
    }
}
