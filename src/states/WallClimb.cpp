#include "WallClimb.h"
#include "../Random.h"

void WallClimb::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        animationMachine.setReferenceTime(time);

        if (shanghai->positionX < static_cast<float>(state->width) / 2) {
            shanghai->positionX -= 40;
        } else {
            shanghai->positionX += 40;
        }
    }

    auto frame = animationMachine.getFrame(time);
    if (!animationMachine.isNewFrame()) return;

    shanghai->setTexture(frame.textureIndex);

    if (descending) {
        shanghai->positionY -= frame.translationY;
    } else {
        shanghai->positionY += frame.translationY;
    }

    // Shanghai falls from the wall
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::FALLING);
    }

    // Shanghai stops climbing
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::WALL_HOLD);
    }
}
