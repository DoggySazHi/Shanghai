#include "WallClimb.h"

void WallClimb::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        animationMachine.setReferenceTime(time);
    }

    auto frame = animationMachine.getFrame(time);
    if (!animationMachine.isNewFrame()) return;

    shanghai->setTexture(frame.textureIndex);

    if (descending) {
        shanghai->positionY -= frame.translationY;
    } else {
        shanghai->positionY += frame.translationY;
    }
}
