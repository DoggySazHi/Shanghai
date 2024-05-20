#include "ShanghaiStateMachine.h"
#include "states/Standing.h"
#include "states/Walking.h"
#include "states/Sitting.h"
#include "states/SittingAndLooking.h"
#include "states/Crawling.h"
#include "states/Ceiling.h"
#include "states/CeilingCrawl.h"


ShanghaiStateMachine::ShanghaiStateMachine() {
    stateActions[ShanghaiState::STANDING] = new Standing();
//    stateActions[ShanghaiState::LOOKING] = new Looking();
    stateActions[ShanghaiState::WALKING] = new Walking();
    stateActions[ShanghaiState::SITTING] = new Sitting();
    stateActions[ShanghaiState::SITTING_AND_LOOKING] = new SittingAndLooking();
    stateActions[ShanghaiState::CRAWLING] = new Crawling();
    stateActions[ShanghaiState::CEILING] = new Ceiling();
    stateActions[ShanghaiState::CEILING_CRAWL] = new CeilingCrawl();
//    stateActions[ShanghaiState::CLIMBING] = new Climbing();
//    stateActions[ShanghaiState::THROWING] = new Throwing();
//    stateActions[ShanghaiState::CHEERING] = new Cheering();
//    stateActions[ShanghaiState::DRAGGED] = new Dragged();
//    stateActions[ShanghaiState::FALLING] = new Falling();
//    stateActions[ShanghaiState::HIT_GROUND] = new HitGround();
}

ShanghaiStateMachine::~ShanghaiStateMachine() {
    for (auto& [shanghaiState, action] : stateActions) {
        delete action;
    }
}

ShanghaiState ShanghaiStateMachine::getState() const {
    return state;
}

void ShanghaiStateMachine::frame(Shanghai* shanghai, EGLState* eglState) {
    stateActions[state]->frame(eglState, shanghai, this);
}

void ShanghaiStateMachine::startDrag(uint32_t x, uint32_t y) {
    // Special case, regardless of the current state, we always start dragging
    dragStartX = x;
    dragStartY = y;
    state = ShanghaiState::DRAGGED;
}

void ShanghaiStateMachine::endDrag() {
    // Special case, regardless of the current state, we always end dragging
    state = ShanghaiState::FALLING;
}
