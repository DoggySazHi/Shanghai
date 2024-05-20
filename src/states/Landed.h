#ifndef SHANGHAI_LANDED_H
#define SHANGHAI_LANDED_H


#include "ShanghaiStateMachineState.h"

class Landed : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    uint64_t lastCycleTime = 0;
    uint8_t step = 0;
#define LANDED_STEP_TIME 67
};


#endif //SHANGHAI_LANDED_H
