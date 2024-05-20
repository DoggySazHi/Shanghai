#ifndef SHANGHAI_DRAGGED_H
#define SHANGHAI_DRAGGED_H


#include "ShanghaiStateMachineState.h"
#include "../animation/ShanghaiAnimationMachine.h"

class Dragged : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;

private:
#define DRAGGED_STEP_TIME 133
    uint64_t lastCycleTime = 0;
    int lastCurX = 0;
    float differenceSmoothed = 0;
    float alpha = 0.1f;
};


#endif //SHANGHAI_DRAGGED_H
