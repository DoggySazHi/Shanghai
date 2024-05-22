#ifndef SHANGHAI_SHANGHAISTATEMACHINE_H
#define SHANGHAI_SHANGHAISTATEMACHINE_H

#include <cstdint>
#include <unordered_map>
#include "states/ShanghaiStateMachineState.h"

enum class ShanghaiState {
    STANDING,
    WALKING,
    SITTING,
    SITTING_AND_LOOKING,
    CRAWLING,
    CEILING,
    CEILING_CRAWL,
    RESIST,
    WALL_HOLD,
    WALL_CLIMB,
    THROWING,
    CHEERING,
    DRAGGED,
    FALLING,
    LANDED,
    JUMP
};

// Forward declaration
class Shanghai;
class ShanghaiStateMachineState;

class ShanghaiStateMachine {
public:
    ShanghaiStateMachine();
    ~ShanghaiStateMachine();
    [[nodiscard]] ShanghaiState getState() const;
    void setState(ShanghaiState newState);
    [[nodiscard]] bool isNewStateFlag() const;
    void startDrag(uint32_t x, uint32_t y);
    void endDrag();

    void frame(Shanghai *shanghai, EGLState *state);

private:
    bool isNewState = true;
    bool isNewStateSet = false;
    uint32_t dragStartX = 0, dragStartY = 0;
    ShanghaiState state = ShanghaiState::SITTING_AND_LOOKING;
    std::unordered_map<ShanghaiState, ShanghaiStateMachineState*> stateActions;

};


#endif //SHANGHAI_SHANGHAISTATEMACHINE_H
