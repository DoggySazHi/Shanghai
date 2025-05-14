#include "Crawling.h"
#include "../Random.h"

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

    // Check if Shanghai is crawling to a wall
    if (shanghai->positionX < 0 || shanghai->positionX + SHANGHAI_TEXTURE_WIDTH >= static_cast<float>(state->width)) {
        machine->setState(ShanghaiState::STANDING);
    }

    // Shanghai is bored
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::SITTING_AND_LOOKING);
    }

    // Shanghai is still bored
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::SITTING);
    }

    // Shanghai wants to walk
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::WALKING);
    }

    // Shanghai wants to stop
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::STANDING);
    }
}
