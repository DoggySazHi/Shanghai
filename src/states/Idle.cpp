#include "Idle.h"

void Idle::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
//    Used for testing mouse follow
//    float h2 = (float) state->height / 2.0f;
//    shanghai->setPos((float) state->curX - 64.0f, h2 - ((float) state->curY - h2) - 64.0f);
    if (Shanghai::getTime() - lastTime > IDLE_FRAME_DELAY) {
        lastTime = Shanghai::getTime();

        currentFrame++;
        if (currentFrame > IDLE_LAST_FRAME) {
            currentFrame = IDLE_FIRST_FRAME;
        }

        shanghai->setTexture(currentFrame);
    }
}

void Idle::startDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}

void Idle::endDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}
