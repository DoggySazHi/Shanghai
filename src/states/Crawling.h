#ifndef SHANGHAI_CRAWLING_H
#define SHANGHAI_CRAWLING_H

#include "ShanghaiStateMachineState.h"

class Crawling : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    float speed = 4.0f;
private:
    uint64_t lastCycleTime = 0;
    uint8_t step = 0;
#define CRAWLING_STEP_TIME 133
};


#endif //SHANGHAI_CRAWLING_H
