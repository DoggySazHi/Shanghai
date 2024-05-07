#ifndef SHANGHAI_SHANGHAISTATEMACHINE_H
#define SHANGHAI_SHANGHAISTATEMACHINE_H

#include <cstdint>
#include <unordered_map>
#include "states/ShanghaiStateMachineState.h"

enum class ShanghaiState {
    IDLE,
    LOOKING,
    WALKING,
    SITTING,
    CLIMBING,
    THROWING,
    CHEERING,
    DRAGGED,
    FALLING,
    HIT_GROUND,
};

// Forward declaration
class Shanghai;
class ShanghaiStateMachineState;

class ShanghaiStateMachine {
public:
    ShanghaiStateMachine();
    ~ShanghaiStateMachine();
    [[nodiscard]] ShanghaiState getState() const;
    void startDrag(uint32_t x, uint32_t y);
    void endDrag();

    void frame(Shanghai *shanghai, EGLState *state);

private:
    uint32_t dragStartX = 0, dragStartY = 0;
    ShanghaiState state = ShanghaiState::IDLE;
    std::unordered_map<ShanghaiState, ShanghaiStateMachineState*> stateActions;

};


#endif //SHANGHAI_SHANGHAISTATEMACHINE_H