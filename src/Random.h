#ifndef SHANGHAI_RANDOM_H
#define SHANGHAI_RANDOM_H

#include <random>

class Random {
public:
    // Return a random float between 0 and 1, [0, 1)
    static float rand() {
        return dist(gen);
    }
private:
    Random() = default;
    static std::random_device rd;
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dist;
};

#endif //SHANGHAI_RANDOM_H
