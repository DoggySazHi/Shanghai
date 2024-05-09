#ifndef SHANGHAI_WALKING_H
#define SHANGHAI_WALKING_H

#include "ShanghaiStateMachineState.h"

class Walking : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void startDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void endDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    uint64_t lastCycleTime = 0;
    uint8_t step = 0;
#define WALK_STEP_TIME 200
#define WALK_STEP_DISTANCE 10
};


#endif //SHANGHAI_WALKING_H
