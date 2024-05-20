#ifndef SHANGHAI_CEILINGCRAWL_H
#define SHANGHAI_CEILINGCRAWL_H

#include "ShanghaiStateMachineState.h"

class CeilingCrawl : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    float speed = 1.0f;
private:
    uint64_t lastCycleTime = 0;
    uint8_t step = 0;
#define CEILING_CRAWL_STEP_TIME 133
};


#endif //SHANGHAI_CEILINGCRAWL_H
