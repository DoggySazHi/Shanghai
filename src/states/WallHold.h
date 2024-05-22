#ifndef SHANGHAI_WALLHOLD_H
#define SHANGHAI_WALLHOLD_H


#include "ShanghaiStateMachineState.h"

class WallHold : public ShanghaiStateMachineState {
public:
    void frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) override;
private:

};


#endif //SHANGHAI_WALLHOLD_H
