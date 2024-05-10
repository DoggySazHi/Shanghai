#include "Walking.h"

void Walking::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
    auto time = Shanghai::getTime();

    // We only care when the step time has passed
    if (time - lastCycleTime < WALK_STEP_TIME) {
        return;
    }

    lastCycleTime = time;

    step = (step + 1) % 4;

    if (step == 0 || step == 2) {
        shanghai->setTexture(0);
    } else if (step == 1) {
        shanghai->setTexture(1);
    } else if (step == 3) {
        shanghai->setTexture(2);
    }

    if (shanghai->flip) {
        shanghai->positionX += speed;
    } else {
        shanghai->positionX -= speed;
    }
}

void Walking::startDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}

void Walking::endDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}
