#ifndef SHANGHAI_LIE_DOWN_H
#define SHANGHAI_LIE_DOWN_H

#include "ShanghaiStateMachineState.h"

class LieDown : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
};


#endif //SHANGHAI_LIE_DOWN_H
