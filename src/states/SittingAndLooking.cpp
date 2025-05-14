#include "SittingAndLooking.h"
#include "../Random.h"

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

    // Shanghai is at the left or right wall
    if (shanghai->positionX < 0 && !shanghai->flip || shanghai->positionX + SHANGHAI_TEXTURE_WIDTH >= static_cast<float>(state->width) && shanghai->flip) {
        if (Random::rand() < 0.001) {
            machine->setState(ShanghaiState::WALL_CLIMB);
        }
    }

    // Shanghai is bored
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::SITTING_AND_LOOKING);
    }

    // Shanghai is still bored
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::SITTING);
    }

    // Shanghai needs to move
    if (Random::rand() < 0.001) {
        shanghai->flip = Random::rand() < 0.5;

        // Flip Shanghai if she's at the left or right wall
        if (shanghai->positionX < 0 && !shanghai->flip || shanghai->positionX + SHANGHAI_TEXTURE_WIDTH >= static_cast<float>(state->width) && shanghai->flip) {
            shanghai->flip = !shanghai->flip;
        } else { // Randomly flip Shanghai
            shanghai->flip = Random::rand() < 0.5;
        }

        if (Random::rand() < 0.5) {
            machine->setState(ShanghaiState::WALKING);
        } else {
            machine->setState(ShanghaiState::CRAWLING);
        }
    }
}
