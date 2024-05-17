#ifndef SHANGHAI_SITTING_H
#define SHANGHAI_SITTING_H


#include "ShanghaiStateMachineState.h"

class Sitting : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
};


#endif //SHANGHAI_SITTING_H
