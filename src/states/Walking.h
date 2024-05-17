#ifndef SHANGHAI_WALKING_H
#define SHANGHAI_WALKING_H

#include "ShanghaiStateMachineState.h"

class Walking : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    float speed = 10.0f;
private:
    uint64_t lastCycleTime = 0;
    uint8_t step = 0;
#define WALK_STEP_TIME 200
};


#endif //SHANGHAI_WALKING_H
