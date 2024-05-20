#include "Falling.h"

void Falling::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        lastCycleTime = time - FALLING_STEP_TIME;
        startFallTime = time;
        startFallY = shanghai->positionY;
    }

//    Technically a bug before I fixed it (below is revised, see previous commits)
//    but it was funnier when the falling was smooth, so I kept it in.
//
//    if (time - lastCycleTime < LANDED_STEP_TIME) {
//        return;
//    }
//    lastCycleTime = time;

    shanghai->setTexture(3);
    shanghai->positionY = kinematicsY((float) time);

    if (shanghai->positionY < 0) {
        shanghai->positionY = 0;
        machine->setState(ShanghaiState::LANDED);
    }
}

float Falling::kinematicsY(float time) const {
    float diff = time - (float) startFallTime;
    return startFallY - (FALLING_STEP_ACC * diff * diff) / 2;
}
