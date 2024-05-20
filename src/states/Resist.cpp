#include "Resist.h"

void Resist::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        animationMachine.setReferenceTime(time);
    }

    auto frame = animationMachine.getFrame(time);
    shanghai->setTexture(frame.textureIndex);
}
