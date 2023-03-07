#include "command.h"
#include "player.h"
#include "world.h"

////////////////////
// Stop
///////////////////
void Stop::execute(Player& player, World& world){
    player.color = {255,0,0,255};
    player.physics.velocity.y = 0;
    player.physics.acceleration.x = 0;
}


///////////////////
// Jump
///////////////////
Jump::Jump(double velocity){
    :velocity{velocity}{}
}

