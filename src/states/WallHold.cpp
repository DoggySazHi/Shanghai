#include "WallHold.h"
#include "../Random.h"

void WallHold::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    shanghai->setTexture(12);

    auto time = Shanghai::getTime();

    // Only run RNG if the time has passed
    if (time - lastCycleTime < WALLHOLD_RNG_TIME) {
        return;
    }

    lastCycleTime = time;

    // Shanghai falls from the wall
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::FALLING);
    }

    // Shanghai returns to climbing
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::WALL_CLIMB);
    }
}
