#ifndef SHANGHAI_STANDING_H
#define SHANGHAI_STANDING_H

#include "ShanghaiStateMachineState.h"

class Standing : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void startDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void endDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
};


#endif //SHANGHAI_STANDING_H
