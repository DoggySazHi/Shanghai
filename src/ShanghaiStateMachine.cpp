#include "ShanghaiStateMachine.h"

ShanghaiState ShanghaiStateMachine::getState() const {
    return ShanghaiState::WALKING;
}

void ShanghaiStateMachine::startDrag(uint32_t x, uint32_t y) {
    dragStartX = x;
    dragStartY = y;
    state = ShanghaiState::DRAGGED;
}

void ShanghaiStateMachine::endDrag() {
    state = ShanghaiState::FALLING;
}
