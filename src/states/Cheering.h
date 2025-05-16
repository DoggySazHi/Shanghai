#ifndef SHANGHAI_CHEERING_H
#define SHANGHAI_CHEERING_H


#include "ShanghaiStateMachineState.h"
#include "../animation/ShanghaiAnimationMachine.h"

class Cheering : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;

private:
    ShanghaiAnimationMachine animationMachine = ShanghaiAnimationMachine
            ({
                     ShanghaiAnimationFrame(37 - 1, 20),
                     ShanghaiAnimationFrame(38 - 1, 20),
                     ShanghaiAnimationFrame(39 - 1, 20),
                     ShanghaiAnimationFrame(40 - 1, 20),
                     ShanghaiAnimationFrame(41 - 1, 20),
             });
};


#endif //SHANGHAI_CHEERING_H
