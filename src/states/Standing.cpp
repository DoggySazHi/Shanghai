#include "Standing.h"

#include <iostream>

#include "../Random.h"

void Standing::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
    shanghai->setTexture(0);

    auto time = Shanghai::getTime();

    // Only run RNG if the time has passed
    if (time - lastCycleTime < STANDING_RNG_TIME) {
        return;
    }

    lastCycleTime = time;

    // Shanghai is at the left or right wall
    if (shanghai->positionX < 0 && !shanghai->flip || shanghai->positionX + SHANGHAI_TEXTURE_WIDTH >= static_cast<float>(state->width) && shanghai->flip) {
        if (Random::rand() < 0.001) {
            machine->setState(ShanghaiState::WALL_CLIMB);
        }
    }

    // Cursor is over Shanghai
    if (shanghai->positionX < state->curX && state->curX < shanghai->positionX + SHANGHAI_TEXTURE_WIDTH && state->height - state->curY < shanghai->positionY + SHANGHAI_TEXTURE_WIDTH * 2) {
        if (Random::rand() < 0.1) {
            machine->setState(ShanghaiState::JUMP);
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
