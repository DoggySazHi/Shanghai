#ifndef SHANGHAI_FALLING_H
#define SHANGHAI_FALLING_H


#include "ShanghaiStateMachineState.h"

class Falling : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;
    float kinematicsY(float time) const;
private:
#define FALLING_STEP_TIME 133
#define FALLING_STEP_ACC 0.01f
    uint64_t lastCycleTime = 0;
    uint64_t startFallTime = 0;
    float startFallY = 0;
};


#endif //SHANGHAI_FALLING_H
