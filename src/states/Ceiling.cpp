#include "Ceiling.h"
#include "../Random.h"

void Ceiling::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    shanghai->setTexture(22);

    auto time = Shanghai::getTime();

    // Only run RNG if the time has passed
    if (time - lastCycleTime < CEILING_RNG_TIME) {
        return;
    }

    lastCycleTime = time;

    // Shanghai falls
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::FALLING);
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

        machine->setState(ShanghaiState::CEILING_CRAWL);
    }
}
