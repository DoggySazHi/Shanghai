#include "CeilingCrawl.h"
#include "../Random.h"

void CeilingCrawl::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        if (shanghai->positionX > static_cast<float>(state->width) - SHANGHAI_TEXTURE_WIDTH - 20) {
            shanghai->flip = true;
            shanghai->positionX = static_cast<float>(state->width) - SHANGHAI_TEXTURE_WIDTH + 10;
        } else if (shanghai->positionX < 20) {
            shanghai->flip = false;
            shanghai->positionX = -10;
        } else {
            shanghai->flip = Random::rand() < 0.5;
        }

        animationMachine.setReferenceTime(time);

        shanghai->positionY = state->height - SHANGHAI_TEXTURE_WIDTH + 30;
    }

    auto frame = animationMachine.getFrame(time);
    if (!animationMachine.isNewFrame()) return;

    shanghai->setTexture(frame.textureIndex);

    if (!shanghai->flip) {
        shanghai->positionX += frame.translationX;
    } else {
        shanghai->positionX -= frame.translationX;
    }

    // Shanghai should fall if she tries to go to a wall
    if (shanghai->positionX < -20) {
        shanghai->positionX = 0;
        machine->setState(ShanghaiState::FALLING);
    } else if (shanghai->positionX > static_cast<float>(state->width) - SHANGHAI_TEXTURE_WIDTH + 20) {
        shanghai->positionX = static_cast<float>(state->width) - SHANGHAI_TEXTURE_WIDTH;
        machine->setState(ShanghaiState::FALLING);
    }

    // Shanghai falls from the wall
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::FALLING);
    }

    // Shanghai stops crawling
    if (Random::rand() < 0.001) {
        machine->setState(ShanghaiState::CEILING);
    }
}
