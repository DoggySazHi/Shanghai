#ifndef SHANGHAI_SHANGHAISTATEMACHINESTATE_H
#define SHANGHAI_SHANGHAISTATEMACHINESTATE_H

#include <cstdint>
#include "../Shanghai.h"
#include "../state.h"

// Forward declaration
class Shanghai;
class ShanghaiStateMachine;

class ShanghaiStateMachineState {
public:
    virtual ~ShanghaiStateMachineState() = default;
    virtual void frame(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) = 0;
    virtual void startDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) = 0;
    virtual void endDrag(EGLState* state, Shanghai* shanghai, ShanghaiStateMachine* machine) = 0;
private:
};

#endif //SHANGHAI_SHANGHAISTATEMACHINESTATE_H
