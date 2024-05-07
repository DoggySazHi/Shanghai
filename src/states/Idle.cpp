#include "Idle.h"

void Idle::frame(EGLState* state, Shanghai *shanghai, ShanghaiStateMachine* machine) {
    float h2 = (float) state->height / 2.0f;
    shanghai->setPos((float) state->curX - 64.0f, h2 - ((float) state->curY - h2) - 64.0f);
}

void Idle::startDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}

void Idle::endDrag(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine* machine) {

}
