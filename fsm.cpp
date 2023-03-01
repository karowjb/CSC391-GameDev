#include "fsm.h"
#include "player.h"
#include "world.h"
////////////
//State
///////////

std::unique_ptr<State> State::update(Player& player, World& world, double dt){
    Physics old = player.physics;
    player.physics.update(dt);
  
    // attempt to move x first then y
    Vec<double> future{player.physics.position.x, old.position.y};
    Vec<double> vx{player.physics.velocity.x,0};
    world.move_to(future, player.size, vx);

    Vec<double> vy{0,player.physics.velocity.y};
    future.y += player.physics.position.y;
    world.move_to(future, player.size, vy);

    //update position and velocity
    player.physics.position = future;
    player.physics.velocity.x = vx.x;
    // landing on a platform
    bool landed = player.physics.velocity.y < 0.0 && vy.y == 0.0;
    player.physics.velocity.y = vy.y;
    if (landed){
        return std::make_unique<Standing>();
    }
    return nullptr;
}


//////////
// Standing
//////////
std::unique_ptr<State> Standing::handle_input(Player& player, const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN){
        SDL_Keycode key = event.key.keysym.sym;
        if (key == SDLK_SPACE || key == SDLK_UP){
            return std::make_unique<Jumping>();
        }
    }
    return nullptr;
}

void Standing::enter(Player& player){
    player.color = {255,0,0,255};
    player.physics.velocity.y = 0;
}

//////////
// Jumping
//////////

std::unique_ptr<State> Jumping::handle_input(Player& player, const SDL_Event& event) {
    return nullptr;
}
void Jumping::enter(Player& player){
    player.color = {0,0,255,255};
    player.physics.velocity.y = player.jump_velocity;
}