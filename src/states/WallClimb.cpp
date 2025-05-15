#include "WallClimb.h"
#include "../Random.h"

void WallClimb::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    const auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        if (shanghai->positionY > static_cast<float>(state->height) - SHANGHAI_TEXTURE_WIDTH) {
            descending = true;
        } else if (shanghai->positionY < 0) {
            descending = false;
        } else {
            descending = Random::rand() < 0.5;
        }

        animationMachine.setReferenceTime(time);

        if (shanghai->positionX < static_cast<float>(state->width) / 2) {
            shanghai->positionX = 0 - 40;
        } else {
            shanghai->positionX = static_cast<float>(state->width) - SHANGHAI_TEXTURE_WIDTH + 40;
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

    if (shanghai->positionY < 0) {
        shanghai->positionY = 0;
        machine->setState(ShanghaiState::STANDING);
    } else if (shanghai->positionY > static_cast<float>(state->height) - SHANGHAI_TEXTURE_WIDTH) {
        shanghai->positionY = static_cast<float>(state->height) - SHANGHAI_TEXTURE_WIDTH;
        machine->setState(ShanghaiState::CEILING_CRAWL);
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
