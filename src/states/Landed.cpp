#include "Landed.h"

void Landed::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        lastCycleTime = time - LANDED_STEP_TIME;
        step = 0;
    }

    if (time - lastCycleTime < LANDED_STEP_TIME) {
        return;
    }

    lastCycleTime = time;
    step++;

    shanghai->flip = false;

    if (step == 4) {
        shanghai->setTexture(18);
        shanghai->positionX += speedToVelocity(shanghai, 8.0f);
    } else if (step == 8) {
        shanghai->setTexture(17);
        shanghai->positionX += speedToVelocity(shanghai, 4.0f);
    } else if (step == 12) {
        shanghai->setTexture(19);
        shanghai->positionX += speedToVelocity(shanghai, 2.0f);
    } else if (step == 16) {
        shanghai->setTexture(19);
    } else if (step == 26) {
        shanghai->setTexture(18);
        shanghai->positionX += speedToVelocity(shanghai, 4.0f);
    } else if (step >= 30) {
        machine->setState(ShanghaiState::STANDING);
    }
}
