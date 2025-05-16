#include "Cheering.h"
#include "../Random.h"

void Cheering::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        animationMachine.setReferenceTime(time);
        shanghai->flip = false;
    }

    auto frame = animationMachine.getFrame(time);
    shanghai->setTexture(frame.textureIndex);

    if (animationMachine.isNewFrame()) {
        if (Random::rand() < 0.03f) {
            machine->setState(ShanghaiState::SITTING);
        }
    }
}
