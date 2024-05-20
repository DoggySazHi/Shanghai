#include "CeilingCrawl.h"

void CeilingCrawl::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    // We only care when the step time has passed
    if (time - lastCycleTime < CEILING_CRAWL_STEP_TIME) {
        return;
    }

    lastCycleTime = time;
    step = (step + 1) % 14;

    // The original textures are facing to the right, so the velocities are inverted

    if (step >= 0 && step <= 3) {
        shanghai->setTexture(24);
    } else if (step == 4) {
        shanghai->setTexture(24);
        shanghai->positionX += speedToVelocity(shanghai, -1.0f * speed);
    } else if (step == 5) {
        shanghai->setTexture(22);
        shanghai->positionX += speedToVelocity(shanghai, -1.0f * speed);
    } else if (step == 6) {
        shanghai->setTexture(23);
        shanghai->positionX += speedToVelocity(shanghai, -1.0f * speed);
    } else if (step >= 7 && step <= 10) {
        shanghai->setTexture(23);
    } else if (step == 11) {
        shanghai->setTexture(23);
        shanghai->positionX += speedToVelocity(shanghai, -2.0f * speed);
    } else if (step == 12) {
        shanghai->setTexture(22);
        shanghai->positionX += speedToVelocity(shanghai, -2.0f * speed);
    } else if (step == 13) {
        shanghai->setTexture(24);
        shanghai->positionX += speedToVelocity(shanghai, -2.0f * speed);
    }
}
