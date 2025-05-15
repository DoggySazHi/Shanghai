#ifndef SHANGHAI_CEILINGCRAWL_H
#define SHANGHAI_CEILINGCRAWL_H

#include "ShanghaiStateMachineState.h"
#include "../animation/ShanghaiAnimationMachine.h"

class CeilingCrawl : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    ShanghaiAnimationMachine animationMachine = ShanghaiAnimationMachine
            ({
                     ShanghaiAnimationFrame(25 - 1, 16, 0, 0),
                     ShanghaiAnimationFrame(25 - 1, 4, 2, 0),
                     ShanghaiAnimationFrame(23 - 1, 4, 2, 0),
                     ShanghaiAnimationFrame(24 - 1, 4, 2, 0),
                     ShanghaiAnimationFrame(24 - 1, 16, 0, 0),
                     ShanghaiAnimationFrame(24 - 1, 4, 4, 0),
                     ShanghaiAnimationFrame(23 - 1, 4, 4, 0),
                     ShanghaiAnimationFrame(25 - 1, 4, 4, 0),
             });
};


#endif //SHANGHAI_CEILINGCRAWL_H
