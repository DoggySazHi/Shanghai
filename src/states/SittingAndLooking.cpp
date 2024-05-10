#include "SittingAndLooking.h"

void SittingAndLooking::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
    auto time = Shanghai::getTime();

    // We only care when the step time has passed
    if (time - lastCycleTime < SITTING_AND_LOOKING_STEP_TIME) {
        return;
    }

    lastCycleTime = time;
    step = (step + 1) % 8;

    if (step == 0 || step == 4) {
        shanghai->setTexture(30);
    } else if (step >= 1 && step <= 3) {
        shanghai->setTexture(31);
    } else if (step >= 5 && step <= 7) {
        shanghai->setTexture(32);
    }
}

void SittingAndLooking::startDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}

void SittingAndLooking::endDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}
