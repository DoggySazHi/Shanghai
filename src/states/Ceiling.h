#ifndef SHANGHAI_CEILING_H
#define SHANGHAI_CEILING_H

#include "ShanghaiStateMachineState.h"

class Ceiling : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    uint64_t lastCycleTime = 0;
#define CEILING_RNG_TIME 200
};


#endif //SHANGHAI_CEILING_H
