#ifndef SHANGHAI_STATE_H
#define SHANGHAI_STATE_H

#include <cstdint>

struct EGLState {
    uint32_t width = 0, height = 0;
    int curX = -1, curY = -1;
    int buttons = 0;

    EGLState() {
        width = 0;
        height = 0;
        curX = -1;
        curY = -1;
        buttons = 0;
    }
};

#endif //SHANGHAI_STATE_H
