#include "Random.h"

std::random_device Random::rd;
std::mt19937 Random::gen = std::mt19937(rd());
std::uniform_real_distribution<float> Random::dist = std::uniform_real_distribution<float>(0, 1);