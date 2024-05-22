#ifndef SHANGHAI_WALLCLIMB_H
#define SHANGHAI_WALLCLIMB_H


#include "ShanghaiStateMachineState.h"
#include "../animation/ShanghaiAnimationMachine.h"

class WallClimb : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;
    bool descending = false;
private:
    ShanghaiAnimationMachine animationMachine = ShanghaiAnimationMachine
            ({
                     ShanghaiAnimationFrame(14 - 1, 16, 0, 0),
                     ShanghaiAnimationFrame(14 - 1, 4, 0, 1),
                     ShanghaiAnimationFrame(12 - 1, 4, 0, 1),
                     ShanghaiAnimationFrame(13 - 1, 4, 0, 1),
                     ShanghaiAnimationFrame(13 - 1, 16, 0, 0),
                     ShanghaiAnimationFrame(13 - 1, 4, 0, 2),
                     ShanghaiAnimationFrame(12 - 1, 4, 0, 2),
                     ShanghaiAnimationFrame(14 - 1, 4, 0, 2),
             });
};


#endif //SHANGHAI_WALLCLIMB_H
