#include "physics.h"

void Physics::update(double dt){
   //update physics, semi-implicit euler
    velocity += acceleration * dt;
    // keep velocity under terminal 
    velocity.x = std::clamp(velocity.x, -terminal_velocity, terminal_velocity);
    velocity.y = std::clamp(velocity.y, -terminal_velocity, terminal_velocity);
    position += velocity * dt;
}