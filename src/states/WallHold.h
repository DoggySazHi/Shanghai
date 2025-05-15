#ifndef SHANGHAI_WALLHOLD_H
#define SHANGHAI_WALLHOLD_H


#include "ShanghaiStateMachineState.h"

class WallHold : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;
private:
    uint64_t lastCycleTime = 0;
#define WALLHOLD_RNG_TIME 200
};


#endif //SHANGHAI_WALLHOLD_H
