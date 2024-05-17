#ifndef SHANGHAI_CEILING_H
#define SHANGHAI_CEILING_H

#include "ShanghaiStateMachineState.h"

class Ceiling : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
};


#endif //SHANGHAI_CEILING_H
