#ifndef SHANGHAI_IDLE_H
#define SHANGHAI_IDLE_H

#include "ShanghaiStateMachineState.h"

class Idle : public ShanghaiStateMachineState {
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void startDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void endDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
};


#endif //SHANGHAI_IDLE_H
