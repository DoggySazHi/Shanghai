#include "Falling.h"
#include "../config/ShanghaiConfiguration.h"
#include "../Random.h"

void Falling::frame(EGLState *state, Shanghai *shanghai, ShanghaiStateMachine *machine) {
    auto time = Shanghai::getTime();

    if (machine->isNewStateFlag()) {
        lastCycleTime = time - FALLING_STEP_TIME;
        velocity = shanghai->velocityY;
    }

//    Technically a bug before I fixed it (below is revised, see previous commits)
//    but it was funnier when the falling was smooth, so I kept it in.
//
//    if (time - lastCycleTime < LANDED_STEP_TIME) {
//        return;
//    }
//    lastCycleTime = time;

    shanghai->setTexture(3);

    velocity = std::min(velocity + FALLING_STEP_ACC, FALLING_MAX_VEL);
    shanghai->positionY -= velocity;

    auto nextXPosition = shanghai->positionX += shanghai->velocityX;
    if (nextXPosition > 0 && nextXPosition < state->width - SHANGHAI_TEXTURE_WIDTH) {
        shanghai->positionX = nextXPosition;
    } else if (nextXPosition < 0) {
        shanghai->positionX *= -0.02f;
        shanghai->velocityX = -shanghai->velocityX;
    } else if (nextXPosition > state->width - SHANGHAI_TEXTURE_WIDTH) {
        shanghai->positionX = (float) (state->width - SHANGHAI_TEXTURE_WIDTH);
        shanghai->velocityX *= -0.02f;
    }

    if (shanghai->positionY < 0) {
        // Try to portal
        if (!portalState && Random::rand() < ShanghaiConfiguration::getInstance()->getFallingPortalProbability()) {
            portalState = true;
        }

        // Failed to portal
        if (!portalState) {
            shanghai->positionY = 0;
            machine->setState(ShanghaiState::LANDED);
        }

        // Successful portal after falling
        if (portalState && shanghai->positionY < -SHANGHAI_TEXTURE_WIDTH) {
            shanghai->positionY = (float) (state->height + SHANGHAI_TEXTURE_WIDTH);
            portalState = false;
        }
    }
}
