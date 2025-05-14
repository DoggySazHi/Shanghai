#include "Walking.h"
#include "../Random.h"

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

    shanghai->positionX += speedToVelocity(shanghai, speed);

    // Check if Shanghai is walking to a wall
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

    // Shanghai wants to crawl
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::CRAWLING);
    }

    // Shanghai wants to stop
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::STANDING);
    }
}
