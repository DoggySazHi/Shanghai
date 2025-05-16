#ifndef SHANGHAI_THROWING_H
#define SHANGHAI_THROWING_H


#include "ShanghaiStateMachineState.h"
#include "../animation/ShanghaiAnimationMachine.h"

class Throwing : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;

private:
    ShanghaiAnimationMachine animationMachine = ShanghaiAnimationMachine
            ({
                     ShanghaiAnimationFrame(42 - 1, 10),
                     ShanghaiAnimationFrame(43 - 1, 10),
                     ShanghaiAnimationFrame(44 - 1, 10),
                     ShanghaiAnimationFrame(45 - 1, 10),
                     ShanghaiAnimationFrame(46 - 1, 10),
             });
};


#endif //SHANGHAI_THROWING_H
