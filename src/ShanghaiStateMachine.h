#ifndef SHANGHAI_SHANGHAISTATEMACHINE_H
#define SHANGHAI_SHANGHAISTATEMACHINE_H

#include <cstdint>

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

class ShanghaiStateMachine {
public:
    [[nodiscard]] ShanghaiState getState() const;
    void startDrag(uint32_t x, uint32_t y);
    void endDrag();
private:
    uint32_t dragStartX = 0, dragStartY = 0;
    ShanghaiState state = ShanghaiState::IDLE;
};


#endif //SHANGHAI_SHANGHAISTATEMACHINE_H
