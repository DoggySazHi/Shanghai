#ifndef SHANGHAI_IDLE_H
#define SHANGHAI_IDLE_H

#include "ShanghaiStateMachineState.h"

class Idle : public ShanghaiStateMachineState {
public:
    void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void startDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
    void endDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) override;
private:
    uint64_t lastTime = 0;
#define IDLE_FRAME_DELAY 1000
#define IDLE_FIRST_FRAME 4
#define IDLE_LAST_FRAME 9
    uint8_t currentFrame = 5;
};


#endif //SHANGHAI_IDLE_H
