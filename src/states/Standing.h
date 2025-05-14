#ifndef SHANGHAI_STANDING_H
#define SHANGHAI_STANDING_H

#include "ShanghaiStateMachineState.h"

class Standing : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    uint64_t lastCycleTime = 0;
#define STANDING_RNG_TIME 200
};


#endif //SHANGHAI_STANDING_H
