#include "Crawling.h"

void Crawling::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
    auto time = Shanghai::getTime();

    // We only care when the step time has passed
    if (time - lastCycleTime < CRAWLING_STEP_TIME) {
        return;
    }

    lastCycleTime = time;
    step = (step + 1) % 16;

    if (step < 7) {
        shanghai->setTexture(19);
    } else if (step < 8) {
        shanghai->setTexture(19);
        shanghai->positionX += speedToVelocity(shanghai, 2.0f * speed);
    } else if (step < 9) {
        shanghai->setTexture(20);
        shanghai->positionX += speedToVelocity(shanghai, 2.0f * speed);
    } else if (step < 10) {
        shanghai->setTexture(20);
        shanghai->positionX += speedToVelocity(shanghai, speed);
    } else {
        shanghai->setTexture(20);
    }
}
