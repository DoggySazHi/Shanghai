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

    // Helper function to convert speed to velocity (respecting the flip flag)
#define speedToVelocity(shanghai, speed) (speed * (shanghai->flip ? 1.0f : -1.0f))
private:
};

#endif //SHANGHAI_SHANGHAISTATEMACHINESTATE_H
