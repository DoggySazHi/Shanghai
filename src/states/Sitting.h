#ifndef SHANGHAI_SITTING_H
#define SHANGHAI_SITTING_H


#include "ShanghaiStateMachineState.h"

class Sitting : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    uint64_t lastCycleTime = 0;
#define SITTING_RNG_TIME 200
};


#endif //SHANGHAI_SITTING_H
