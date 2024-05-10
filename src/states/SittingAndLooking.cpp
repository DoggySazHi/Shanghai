#include "SittingAndLooking.h"

void SittingAndLooking::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
    auto time = Shanghai::getTime();

    // We only care when the step time has passed
    if (time - lastCycleTime < SITTING_AND_LOOKING_STEP_TIME) {
        return;
    }

    lastCycleTime = time;
    step = (step + 1) % 8;

    if (step == 0) {
        shanghai->setTexture(25);
    } else if (step == 1) {
        shanghai->setTexture(14);
    } else if (step == 2) {
        shanghai->setTexture(26);
    } else if (step == 3) {
        shanghai->setTexture(15);
    } else if (step == 4) {
        shanghai->setTexture(27);
    } else if (step == 5) {
        shanghai->setTexture(16);
    } else if (step == 6) {
        shanghai->setTexture(28);
    } else if (step == 7) {
        shanghai->setTexture(17);
    }
}

void SittingAndLooking::startDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}

void SittingAndLooking::endDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}
