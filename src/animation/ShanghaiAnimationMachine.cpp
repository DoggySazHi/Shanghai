#include <stdexcept>
#include "ShanghaiAnimationMachine.h"

ShanghaiAnimationMachine::ShanghaiAnimationMachine(const std::vector<ShanghaiAnimationFrame> &frames) {
    this->frames = frames;

    for (auto& frame : frames) {
        totalDuration += frame.duration;
    }

    totalDuration *= durationFactor;

    // Avoid division by zero
    if (totalDuration == 0) {
        totalDuration = 1;
    }
}

void ShanghaiAnimationMachine::setAnimationFactor(uint32_t factor) {
    this->durationFactor = factor;
}

void ShanghaiAnimationMachine::setReferenceTime(uint64_t time) {
    this->referenceTime = time;
}

ShanghaiAnimationFrame &ShanghaiAnimationMachine::getFrame(uint64_t time) {
    // Time is expected to be in milliseconds
    auto elapsedTime = time - referenceTime; // Offset the time to the reference time
    elapsedTime = elapsedTime % totalDuration; // Loop the animation

    for (auto& frame : frames) {
        if (elapsedTime < frame.duration * durationFactor) {
            return frame;
        }

        elapsedTime -= frame.duration * durationFactor;
    }

    // This should never happen
    throw std::runtime_error("Animation machine failed to get a frame - mukyu!");
}
