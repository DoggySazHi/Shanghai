#ifndef SHANGHAI_SITTING_AND_WALKING_H
#define SHANGHAI_SITTING_AND_WALKING_H

#include "ShanghaiStateMachineState.h"

class SittingAndLooking : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    float speed = 10.0f;
private:
    uint64_t lastCycleTime = 0;
    uint8_t step = 0;
#define SITTING_AND_LOOKING_STEP_TIME 167
};


#endif //SHANGHAI_SITTING_AND_WALKING_H
