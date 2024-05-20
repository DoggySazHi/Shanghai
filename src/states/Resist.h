#ifndef SHANGHAI_RESIST_H
#define SHANGHAI_RESIST_H


#include "ShanghaiStateMachineState.h"
#include "../animation/ShanghaiAnimationMachine.h"

class Resist : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    ShanghaiAnimationMachine animationMachine = ShanghaiAnimationMachine({
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(1 - 1, 50),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(1 - 1, 100),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 5),
         ShanghaiAnimationFrame(6 - 1, 5),
         ShanghaiAnimationFrame(5 - 1, 2),
         ShanghaiAnimationFrame(6 - 1, 2),
         ShanghaiAnimationFrame(5 - 1, 2),
         ShanghaiAnimationFrame(6 - 1, 2),
         ShanghaiAnimationFrame(5 - 1, 2),
         ShanghaiAnimationFrame(6 - 1, 2),
         ShanghaiAnimationFrame(5 - 1, 2),
         ShanghaiAnimationFrame(6 - 1, 2),
    });
};


#endif //SHANGHAI_RESIST_H
