#include "command.h"
#include "player.h"
#include "world.h"

////////////////////
// Stop
///////////////////
void Stop::execute(Player& player, World& ){
    player.color = {255,0,0,255};
    player.physics.velocity.y = 0;
    player.physics.acceleration.x = 0;
}

///////////////////
// Jump
///////////////////
Jump::Jump(double velocity)
    :velocity{velocity}{}

void Jump::execute(Player& player, World& ){
    player.color = {0,0,255,255};
    player.physics.velocity.y = velocity;
}

///////////////////
// Accelerate
///////////////////
Accelerate::Accelerate(double acceleration)
    :acceleration{acceleration}{}

void Accelerate::execute(Player& player, World& ){
    player.color = {0,51,0,255};
    player.physics.acceleration.x = acceleration;
}

///////////////////
// Slide
///////////////////
Slide::Slide(double speed)
    :speed{speed}{}

void Slide::execute(Player& player, World& ){
    player.color = {0,204,204,255};
    player.physics.acceleration.x = speed;
}

///////////////////
// Shoot
///////////////////
Shoot::Shoot(double speed)
    :speed{speed}{}

void Shoot::execute(Player& player, World&){
    player.color = {230,76,0,255};
    player.physics.acceleration.x = speed;
}

///////////////////
// Fall
///////////////////
Fall::Fall(double speed)
    :speed{speed}{}

void Fall::execute(Player& player, World&){
    player.color = {125,125,125,255};
    player.physics.acceleration.y = speed;
}