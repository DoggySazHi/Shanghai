#ifndef SHANGHAI_FALLING_H
#define SHANGHAI_FALLING_H


#include "ShanghaiStateMachineState.h"

class Falling : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;
private:
#define FALLING_STEP_TIME 133
#define FALLING_STEP_ACC 0.5f
#define FALLING_MAX_VEL 200.0f
    uint64_t lastCycleTime = 0;
    bool portalState = false;
    float velocity = 0;
};


#endif //SHANGHAI_FALLING_H
