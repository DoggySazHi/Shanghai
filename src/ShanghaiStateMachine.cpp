#include <iostream>
#include <utility>
#include "ShanghaiStateMachine.h"
#include "states/Standing.h"
#include "states/Walking.h"
#include "states/Sitting.h"
#include "states/SittingAndLooking.h"
#include "states/Crawling.h"
#include "states/Ceiling.h"
#include "states/CeilingCrawl.h"
#include "states/Resist.h"
#include "states/Dragged.h"
#include "states/Falling.h"
#include "states/Landed.h"


ShanghaiStateMachine::ShanghaiStateMachine() {
    stateActions[ShanghaiState::STANDING] = new Standing();
//    stateActions[ShanghaiState::LOOKING] = new Looking();
    stateActions[ShanghaiState::WALKING] = new Walking();
    stateActions[ShanghaiState::SITTING] = new Sitting();
    stateActions[ShanghaiState::SITTING_AND_LOOKING] = new SittingAndLooking();
    stateActions[ShanghaiState::CRAWLING] = new Crawling();
    stateActions[ShanghaiState::CEILING] = new Ceiling();
    stateActions[ShanghaiState::CEILING_CRAWL] = new CeilingCrawl();
    stateActions[ShanghaiState::RESIST] = new Resist();
//    stateActions[ShanghaiState::CLIMBING] = new Climbing();
//    stateActions[ShanghaiState::THROWING] = new Throwing();
//    stateActions[ShanghaiState::CHEERING] = new Cheering();
    stateActions[ShanghaiState::DRAGGED] = new Dragged();
    stateActions[ShanghaiState::FALLING] = new Falling();
    stateActions[ShanghaiState::LANDED] = new Landed();
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

    // We require two states because we need to know if the state has changed (e.g., a transition)
    // Allows us to propagate the isNewState flag to the next state
    if (isNewStateSet) {
        isNewStateSet = false;
    } else if (isNewState) {
        isNewState = false;
    }

    // Handle mouse inputs
    if (shanghai->inShanghai(eglState) && state != ShanghaiState::DRAGGED && eglState->buttons) {
        startDrag(eglState->curX, eglState->curY);
    } else if (state == ShanghaiState::DRAGGED && !eglState->buttons) {
        endDrag();
    }
}

void ShanghaiStateMachine::startDrag(uint32_t x, uint32_t y) {
    // Special case, regardless of the current state, we always start dragging
    dragStartX = x;
    dragStartY = y;
    setState(ShanghaiState::DRAGGED);
}

void ShanghaiStateMachine::endDrag() {
    // Special case, regardless of the current state, we always end dragging
    setState(ShanghaiState::FALLING);
}

void ShanghaiStateMachine::setState(ShanghaiState newState) {
#ifdef DEBUG
    if (state != newState) {
        std::cout << "State change: " << std::to_underlying(state) << " -> " << std::to_underlying(newState) << std::endl;
    }
#endif

    state = newState;
    isNewState = true;
    isNewStateSet = true;
}

bool ShanghaiStateMachine::isNewStateFlag() const {
    return isNewState;
}
