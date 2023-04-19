#pragma once
#include "vec.h"

constexpr double terminal_velocity = 30;
constexpr double gravity = -10;
constexpr double damping = 0.92;

class Physics {
public:
    void update(double dt);
    Vec<double> position, velocity, acceleration{0,0};
};