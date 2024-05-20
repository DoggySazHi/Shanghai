#include "Falling.h"

void Falling::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        lastCycleTime = time - FALLING_STEP_TIME;
        startFallTime = time;
        startFallY = shanghai->positionY;
    }

    if (time - lastCycleTime > FALLING_STEP_TIME) {
        lastCycleTime = time;
    }

    shanghai->setTexture(3);
    shanghai->positionY = kinematicsY((float) time);
}

float Falling::kinematicsY(float time) const {
    float diff = time - (float) startFallTime;
    return startFallY - (FALLING_STEP_ACC * diff * diff) / 2;
}
