#ifndef SHANGHAI_JUMP_H
#define SHANGHAI_JUMP_H


#include "ShanghaiStateMachineState.h"

class Jump : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
#define JUMP_STEP_ACC 0.5f
#define JUMP_INITIAL_VELOCITY 10.0f
    float velocity = 0.0f;
};


#endif //SHANGHAI_JUMP_H
