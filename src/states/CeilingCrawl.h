#ifndef SHANGHAI_CEILINGCRAWL_H
#define SHANGHAI_CEILINGCRAWL_H

#include "ShanghaiStateMachineState.h"

class CeilingCrawl : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
};


#endif //SHANGHAI_CEILINGCRAWL_H
